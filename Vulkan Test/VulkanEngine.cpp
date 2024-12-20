#include "VulkanEngine.h"

#include <stdexcept>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>

#include "VulkanContext.h"
#include "VulkanDeviceManager.h"
#include "VulkanPipeline.h"
#include "VulkanRenderer.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

vk::Device VulkanEngine::LogicalDevice = nullptr;
vk::PhysicalDevice VulkanEngine::PhysicalDevice = VK_NULL_HANDLE;
vk::CommandPool VulkanEngine::MainCommandPool;
QueueFamilyIndices VulkanEngine::FamilyIndices;

vk::Queue VulkanEngine::GraphicsQueue;
vk::Queue VulkanEngine::KHRPresentQueue;


void VulkanEngine::Run()
{
	InitContext();
	InitVulkan();
	MainLoop();
	Cleanup();
}

void VulkanEngine::LoadMesh(aiMesh* _mesh)
{
	m_vulkanPipeline->Load(_mesh);
}

void VulkanEngine::InitContext()
{
	m_context = new VulkanContext();
	ContextInfo info = {
		.name = "Vulkan",
		.width = WIDTH,
		.height = HEIGHT,
		.hints = {{GLFW_CLIENT_API, GLFW_NO_API}, {GLFW_RESIZABLE, GLFW_FALSE}}
	};
	m_context->Init(info);
	m_instance = m_context->CreateInstance(GetAppInfo());
	m_context->CreateSurfaceKHR(&m_renderingSurface);
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
	m_deviceManager = new VulkanDeviceManager(m_renderingSurface);
	m_deviceManager->Init(m_instance);

	GraphicsQueue = VulkanEngine::LogicalDevice.getQueue(FamilyIndices.graphicsFamily.value(), 0);
	KHRPresentQueue = VulkanEngine::LogicalDevice.getQueue(FamilyIndices.khrPresentFamily.value(), 0);
	CreateMainCommandPool();

	SwapChainSupportDetails details = m_deviceManager->GetSwapChainSupportDetails();
	VulkanPipelineInfo pipelineInfo = {
		.details = details,
		.surface = m_renderingSurface
	};
	m_vulkanPipeline = new VulkanPipeline(pipelineInfo);
	extent = m_context->GetExtent(details.capabilities);

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile("C:/Models/test.fbx", aiProcess_Triangulate);

	m_vulkanPipeline->Init(extent, scene->mMeshes[0]);
	m_swapChain = m_vulkanPipeline->GetSwapChain();
	
	m_vulkanRenderer = new VulkanRenderer(extent, m_vulkanPipeline->GetFrameBuffers());
	m_vulkanRenderer->Init(m_vulkanPipeline->GetDescriptorSetLayout());
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

void VulkanEngine::MainLoop()
{
	while (!m_context->ShouldClose())
	{
		m_context->PollEvents();
		Render();
	}
}

void VulkanEngine::Render()
{
	m_vulkanRenderer->Render(m_swapChain, m_vulkanPipeline->GetRenderInfo(), m_vulkanPipeline->GetRenderPass());
}

void VulkanEngine::Cleanup()
{
	VulkanEngine::LogicalDevice.destroySwapchainKHR(m_swapChain);
	VulkanEngine::LogicalDevice.destroy();
	m_instance.destroySurfaceKHR(m_renderingSurface);
	m_instance.destroy();
}
