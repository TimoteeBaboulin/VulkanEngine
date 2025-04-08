#include "MoonlitVulkan/VulkanEngine.h"

#include <stdexcept>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>

#include "MoonlitVulkan/VulkanContext.h"
#include "MoonlitVulkan/VulkanDeviceManager.h"
#include "MoonlitVulkan/VulkanPipeline.h"
#include "MoonlitVulkan/VulkanRenderer.h"
#include "Inputs/InputManager.h"

vk::Device VulkanEngine::LogicalDevice = nullptr;
vk::PhysicalDevice VulkanEngine::PhysicalDevice = VK_NULL_HANDLE;
vk::CommandPool VulkanEngine::MainCommandPool;
QueueFamilyIndices VulkanEngine::FamilyIndices;

vk::Queue VulkanEngine::GraphicsQueue;
vk::Queue VulkanEngine::KHRPresentQueue;

//void VulkanEngine::Run()
//{
//	InitContext();
//	InitVulkan();
//	MainLoop();
//	Cleanup();
//}

void VulkanEngine::LoadMesh(MeshData& _mesh)
{
	m_vulkanRenderer->LoadMesh(_mesh);
}

//VKAPI_ATTR VkBool32 VKAPI_CALL debug_utils_messenger_callback(
//	VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
//	VkDebugUtilsMessageTypeFlagsEXT message_type,
//	const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
//	void* user_data)
//{
//	if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
//	{
//		std::cout << callback_data->messageIdNumber << " - " << callback_data->pMessageIdName << ": " << callback_data->pMessage << std::endl;
//	}
//	else if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
//	{
//		std::cout << callback_data->messageIdNumber << " - " << callback_data->pMessageIdName << ": " << callback_data->pMessage << std::endl;
//	}
//	return VK_FALSE;
//}

void VulkanEngine::InitContext(ContextInfo& _info, const char** requiredExtensions, int extensionCount)
{
	m_context = new VulkanContext();
	m_context->Init(_info);

	vk::ApplicationInfo appInfo = GetAppInfo();
	InputManager::InitManager(_info.windowHandle);

//#ifndef NDEBUG
//	vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo;
//	debugCreateInfo.sType = vk::StructureType::eDebugUtilsMessengerCreateInfoEXT;
//	debugCreateInfo.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning;
//	debugCreateInfo.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation;
//	debugCreateInfo.pfnUserCallback = &debug_utils_messenger_callback;
//	appInfo.pNext = &debugCreateInfo;
//#endif

	m_instance = m_context->CreateInstance(appInfo, requiredExtensions, extensionCount);

//#ifndef NDEBUG
//	m_instance.createDebugUtilsMessengerEXT(debugCreateInfo, nullptr, vk::DispatchLoaderDynamic({ *m_instance }));
//#endif // !NDEBUG

	m_context->CreateSurfaceKHR();
}

vk::ApplicationInfo VulkanEngine::GetAppInfo()
{
	vk::ApplicationInfo appInfo{};
	appInfo.sType = vk::StructureType::eApplicationInfo;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	return appInfo;
}

void VulkanEngine::InitVulkan()
{
	vk::Extent2D extent;
	m_deviceManager = new VulkanDeviceManager(*m_context->GetSurface());
	m_deviceManager->Init(m_instance);

	GraphicsQueue = VulkanEngine::LogicalDevice.getQueue(FamilyIndices.graphicsFamily.value(), 0);
	KHRPresentQueue = VulkanEngine::LogicalDevice.getQueue(FamilyIndices.khrPresentFamily.value(), 0);
	CreateMainCommandPool();

	SwapChainSupportDetails details = m_deviceManager->GetSwapChainSupportDetails();
	/*VulkanPipelineInfo pipelineInfo = {
		.details = details,
		.surface = m_renderingSurface
	};*/
	//m_vulkanPipeline = new VulkanPipeline(pipelineInfo);
	extent = m_context->GetExtent(details.capabilities);

	//m_vulkanPipeline->Init(extent);
	//m_swapChain = m_vulkanPipeline->GetSwapChain();
	
	m_vulkanRenderer = new VulkanRenderer(this, extent);
	m_vulkanRenderer->Init(m_context, m_deviceManager);
}

vk::PresentModeKHR VulkanEngine::GetPresentMode(std::vector<vk::PresentModeKHR>& _modes)
{
	if (std::find(_modes.begin(), _modes.end(), vk::PresentModeKHR::eMailbox) != _modes.end())
		return vk::PresentModeKHR::eMailbox;

	std::cout << "Can't use mailbox mode switching to fifo" << std::endl;
	return vk::PresentModeKHR::eFifo;
}

vk::SurfaceFormatKHR VulkanEngine::GetFormat(std::vector<vk::SurfaceFormatKHR>& _format)
{
	return _format[0];
}

void VulkanEngine::CreateMainCommandPool()
{
	vk::CommandPoolCreateInfo poolInfo{};
	poolInfo.sType = vk::StructureType::eCommandPoolCreateInfo;
	poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
	poolInfo.queueFamilyIndex = FamilyIndices.graphicsFamily.value();

	MainCommandPool = LogicalDevice.createCommandPool(poolInfo);
}

void VulkanEngine::Render()
{
	m_vulkanRenderer->Render();
}

void VulkanEngine::Cleanup()
{
	VulkanEngine::LogicalDevice.destroySwapchainKHR(m_swapChain);
	VulkanEngine::LogicalDevice.destroy();
	//TODO: Destroy the surface in context
	//m_instance.destroySurfaceKHR(m_renderingSurface);
	m_instance.destroy();
}
