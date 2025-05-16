#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include "vulkan/vulkan.hpp"
#include "common.h"

#include "MoonlitVulkanExport.h"
#include "Renderer/ContextManager.h"

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

class VulkanContext;
class VulkanDeviceManager;
class Renderer;
class VulkanPipeline;

class MOONLIT_API VulkanEngine
{
public:
	//void Run();
	void InitContext(ContextInfo& _info, const char** requiredExtensions, int extensionCount);
	void InitVulkan();

	//void MainLoop();
	void Render();

	void LoadMesh(std::string _name);

	VulkanContext* GetContext() const { return m_context; }
	HWND GetWindow() const { return m_context->GetWindow(); }

	static vk::Device LogicalDevice;
	static vk::PhysicalDevice PhysicalDevice;

	static vk::CommandPool MainCommandPool;
	static QueueFamilyIndices FamilyIndices;

	static vk::Queue GraphicsQueue;
	static vk::Queue KHRPresentQueue;

	static float TimeStampPeriods;

private:
	VulkanContext* m_context = nullptr;
	vk::Instance m_instance;

	VulkanDeviceManager* m_deviceManager = nullptr;

	//VulkanPipeline* m_vulkanPipeline = nullptr;
	vk::SwapchainKHR m_swapChain;

	Renderer* m_vulkanRenderer = nullptr;
	
	vk::ApplicationInfo GetAppInfo();

	//Helpers
	vk::PresentModeKHR GetPresentMode(std::vector<vk::PresentModeKHR>& _modes);
	vk::SurfaceFormatKHR GetFormat(std::vector<vk::SurfaceFormatKHR>& _format);


	void CreateMainCommandPool();
	void Cleanup();
};