#pragma once

#include "vulkan/vulkan.hpp"
#include "common.h"
#include "assimp/scene.h"

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

class VulkanContext;
class VulkanDeviceManager;
class VulkanRenderer;
class VulkanPipeline;

class MOONLIT_API VulkanEngine
{
public:
	void Run();

	void LoadMesh(aiMesh* _mesh);

	static vk::Device LogicalDevice;
	static vk::PhysicalDevice PhysicalDevice;

	static vk::CommandPool MainCommandPool;
	static QueueFamilyIndices FamilyIndices;

	static vk::Queue GraphicsQueue;
	static vk::Queue KHRPresentQueue;

private:
	VulkanContext* m_context;
	vk::Instance m_instance;
	vk::SurfaceKHR m_renderingSurface;

	VulkanDeviceManager* m_deviceManager;

	VulkanPipeline* m_vulkanPipeline;
	vk::SwapchainKHR m_swapChain;

	VulkanRenderer* m_vulkanRenderer;
	

	void InitContext();
	vk::ApplicationInfo GetAppInfo();

	//Vulkan
	void InitVulkan();

	//Helpers
	vk::PresentModeKHR GetPresentMode(std::vector<vk::PresentModeKHR>& _modes);
	vk::SurfaceFormatKHR GetFormat(std::vector<vk::SurfaceFormatKHR>& _format);


	void CreateMainCommandPool();
	void MainLoop();
	void Render();
	void Cleanup();
};