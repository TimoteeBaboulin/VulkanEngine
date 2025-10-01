#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>
#include <Windows.h>
#include <vector>

#include "RendererDeviceManager.h"
#include "Camera.h"

class RendererContext;
class DrawBuffer;

class RenderTarget
{
public:
	RenderTarget(int _framesInFlight, HWND _targetWindow, 
		vk::Instance _instance, Camera* _camera);

	void LinkDeviceManager(RendererDeviceManager* _deviceManager);
	void Init(vk::DescriptorSetLayout _uboLayout,
		vk::RenderPass _renderPass);
	void SetRenderPass(vk::RenderPass _renderPass);
	void Render(std::vector<DrawBuffer> _drawBuffers);

#pragma region Getters
	vk::Format GetFormat() const { return m_format.format; }
	vk::SurfaceKHR GetSurfaceKHR() const { return m_surfaceKHR; }
	vk::CommandPool GetCommandPool() const { return m_commandPool; }

	vk::Device GetDevice() const { return m_device; }
	vk::PhysicalDevice GetPhysicalDevice() const { return m_physDevice; }
#pragma endregion //Getters

private:
	size_t m_framesInFlight = 0;
	int m_currentFrame = 0;

	HWND m_targetWindow = nullptr;

	RenderQueues m_queues;

	vk::Instance m_instance;
	vk::Device m_device;
	vk::PhysicalDevice m_physDevice;

	RendererDeviceManager* m_deviceManager = nullptr;
	Camera* m_camera = nullptr;

#pragma region Surface Data
	vk::Extent2D m_extent;
	vk::SurfaceFormatKHR m_format;
	vk::PresentModeKHR m_presentMode;
	vk::SurfaceCapabilitiesKHR m_capabilities;
	vk::SurfaceKHR m_surfaceKHR;
#pragma endregion //Surface Data

#pragma region Swapchain Data
	vk::SwapchainKHR m_swapChain;
	// The images are stored in a vector due to vulkan giving us a vector.
	std::vector<vk::Image> m_swapChainImages;
	vk::ImageView* m_swapChainImageViews = nullptr;

	vk::Image* m_swapChainDepthImages = nullptr;
	vk::ImageView* m_swapChainDepthImageViews = nullptr;
	vk::DeviceMemory m_swapChainDepthMemory;

	vk::Framebuffer* m_swapChainFramebuffers = nullptr;
	vk::RenderPass m_renderPass;
#pragma endregion //Swapchain Data

#pragma region Synchronization Resources
	vk::Semaphore* m_imageAvailableSemaphores = nullptr;
	vk::Semaphore* m_renderFinishedSemaphores = nullptr;
	vk::Fence* m_waitForPreviousFrame = nullptr;
#pragma endregion //Synchronization Resources

#pragma region Uniform Buffers Resources
	vk::Buffer* m_uniformBuffers = nullptr;
	vk::DeviceMemory* m_uniformBuffersMemory = nullptr;
	void** m_uniformBuffersMaps = nullptr;

	vk::DescriptorSetLayout m_uboDescriptorSetLayout;
	vk::DescriptorPool m_descriptorPool;
	std::vector<vk::DescriptorSet> m_descriptorSets;
#pragma endregion //Uniform Buffers Resources

	vk::CommandBuffer* m_commandBuffers = nullptr;
	vk::CommandPool m_commandPool;

#pragma region Surface Methods
	void CalculateExtent();
	void CreateSurfaceKHR();
#pragma endregion //Surface Methods

#pragma region Swapchain Methods
	void CreateSwapChainResources();
	void CreateSwapChain();
	void CreateImageViews();
	void CreateDepthResources();
	void CreateFrameBuffers();
	void RecreateSwapChain();
	void DestroySwapChain();
#pragma endregion //Swapchain Methods

#pragma region Uniform Buffers Methods
	void CreateUniformBuffers();
	void UpdateUniformBuffer();

	//void CreateDescriptorSetLayout();
	void CreateDescriptorPool();
	void CreateDescriptorSets();
#pragma endregion //Uniform Buffers Methods

	void CreateSyncObjects();

	void CreateCommandPool();
	void CreateCommandBuffers();
	void RecordCommandBuffer(vk::CommandBuffer& _buffer, std::vector<DrawBuffer>& _drawBuffers);
};