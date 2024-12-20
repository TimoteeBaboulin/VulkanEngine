#include "VulkanContext.h"

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

vk::Instance VulkanContext::CreateInstance(vk::ApplicationInfo _appInfo)
{
	unsigned int extensionCount;

	vk::ApplicationInfo appInfo = _appInfo;

	const char** extensions = glfwGetRequiredInstanceExtensions(&extensionCount);

	vk::InstanceCreateInfo createInfo{};
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = extensionCount;
	createInfo.ppEnabledExtensionNames = extensions;

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
