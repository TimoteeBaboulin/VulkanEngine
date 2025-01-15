#include "VulkanContext.h"
#include "VulkanEngine.h"

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
	glfwInit();
	for (std::pair<int,int> pair : _info.hints)
	{
		glfwWindowHint(pair.first, pair.second);
	}

	m_window = glfwCreateWindow(_info.width, _info.height, _info.name, nullptr, nullptr);
}

void VulkanContext::Cleanup()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
	m_window = nullptr;
}

bool CheckValidationLayerSupport()
{
	uint32_t layerCount = 0;
	vk::enumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<vk::LayerProperties> availableLayers(layerCount);
	vk::enumerateInstanceLayerProperties(&layerCount, availableLayers.data());

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

vk::Instance VulkanContext::CreateInstance(vk::ApplicationInfo _appInfo)
{
	unsigned int extensionCount;

	vk::ApplicationInfo appInfo = _appInfo;

	const char** extensions = glfwGetRequiredInstanceExtensions(&extensionCount);

	if (ValidationLayersEnabled && !CheckValidationLayerSupport())
	{
		std::cout << "Couldn't activate validation layers." << std::endl;
		ValidationLayersEnabled = false;
	}

	vk::InstanceCreateInfo createInfo{};
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = extensionCount;
	createInfo.ppEnabledExtensionNames = extensions;

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

void VulkanContext::CreateSurfaceKHR(vk::SurfaceKHR* _out)
{
	glfwCreateWindowSurface(m_instance, m_window, nullptr, (VkSurfaceKHR*)_out);
}

bool VulkanContext::ShouldClose() const
{
	return glfwWindowShouldClose(m_window);
}

void VulkanContext::PollEvents() const
{
	glfwPollEvents();
}

vk::Extent2D VulkanContext::GetExtent(vk::SurfaceCapabilitiesKHR _capabilities) const
{
	if (_capabilities.currentExtent != std::numeric_limits<uint32_t>::max())
		return _capabilities.currentExtent;

	int width;
	int height;
	glfwGetFramebufferSize(m_window, &width, &height);

	uint32_t uwidth = (uint32_t)width;
	uint32_t uheight = (uint32_t)height;

	uwidth = std::clamp(uwidth, _capabilities.minImageExtent.width, _capabilities.maxImageExtent.width);
	uheight = std::clamp(uheight, _capabilities.minImageExtent.height, _capabilities.maxImageExtent.height);

	return vk::Extent2D(uwidth, uheight);
}
