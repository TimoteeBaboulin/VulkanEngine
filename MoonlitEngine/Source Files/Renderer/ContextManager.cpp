#include "Renderer/ContextManager.h"
#include "Renderer/VulkanEngine.h"

#ifdef NDEBUG
bool VulkanContext::ValidationLayersEnabled = false;
#else
bool VulkanContext::ValidationLayersEnabled = true;
#endif // NDEBUG

const std::vector<const char*> requiredLayers = {
	"VK_LAYER_KHRONOS_validation"
};

void VulkanContext::Init(const ContextInfo& _info)
{
	m_extent.width = _info.width;
	m_extent.height = _info.height;

	m_windowName = _info.name;
	m_window = _info.windowHandle;
}

void VulkanContext::Cleanup()
{
	m_window = nullptr;
}

bool CheckValidationLayerSupport()
{
	uint32_t layerCount = 0;
	vk::Result result;

	result = vk::enumerateInstanceLayerProperties(&layerCount, nullptr);

	if (result != vk::Result::eSuccess)
	{
		throw new std::runtime_error("Couldn't enumerate layer properties.");
	}

	std::vector<vk::LayerProperties> availableLayers(layerCount);
	result = vk::enumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	if (result != vk::Result::eSuccess)
	{
		throw new std::runtime_error("Couldn't enumerate layer properties on second run.");
	}

	for (const char* layerName : requiredLayers)
	{
		bool layerFound = false;
		for (const auto layer : availableLayers)
		{
			if (strcmp(layer.layerName, layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound) return false;
	}

	return true;
}

vk::Instance VulkanContext::CreateInstance(vk::ApplicationInfo _appInfo, const char ** requiredExtensions, int extensionCount)
{
	vk::ApplicationInfo appInfo = _appInfo;

	if (ValidationLayersEnabled && !CheckValidationLayerSupport())
	{
		std::cout << "Couldn't activate validation layers." << std::endl;
		ValidationLayersEnabled = false;
	}

	vk::InstanceCreateInfo createInfo{};
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = extensionCount;
	createInfo.ppEnabledExtensionNames = requiredExtensions;

	if (VulkanContext::ValidationLayersEnabled)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size());
		createInfo.ppEnabledLayerNames = requiredLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	m_instance = vk::createInstance(createInfo);
	return m_instance;
}

void VulkanContext::CreateSurfaceKHR()
{
	vk::Win32SurfaceCreateInfoKHR surfaceCreate;
	surfaceCreate.sType = vk::StructureType::eWin32SurfaceCreateInfoKHR;
	surfaceCreate.hwnd = m_window;
	surfaceCreate.hinstance = GetModuleHandle(nullptr);

	m_surface = m_instance.createWin32SurfaceKHR(surfaceCreate);
}

//bool VulkanContext::ShouldClose() const
//{
//	return glfwWindowShouldClose(m_window);
//}
//
//void VulkanContext::PollEvents() const
//{
//	glfwPollEvents();
//}

vk::Extent2D VulkanContext::GetSurfaceExtent(vk::PhysicalDevice physicalDevice)
{
	vk::SurfaceCapabilitiesKHR capabilities = physicalDevice.getSurfaceCapabilitiesKHR(m_surface);
	return GetExtent(capabilities);
}

vk::Extent2D VulkanContext::GetExtent(vk::SurfaceCapabilitiesKHR _capabilities)
{
	if (_capabilities.currentExtent != std::numeric_limits<uint32_t>::max())
		return _capabilities.currentExtent;

	int width = m_extent.width;
	int height = m_extent.height;

	uint32_t uwidth = (uint32_t)width;
	uint32_t uheight = (uint32_t)height;

	uwidth = std::clamp(uwidth, _capabilities.minImageExtent.width, _capabilities.maxImageExtent.width);
	uheight = std::clamp(uheight, _capabilities.minImageExtent.height, _capabilities.maxImageExtent.height);

	m_extent.width = uwidth;
	m_extent.height = uheight;

	return vk::Extent2D(uwidth, uheight);
}
