#pragma once

#include <Windows.h>
#include <vector>
#include <map>

#include "CustomVulkanStructs.h"
#include "RendererDeviceManager.h"
#include "Camera.h"

class RendererContext;
class DrawBuffer;
class Material;

struct SubpassInfo
{
	std::string subpassName;
	uint32_t subpassIndex;
};

// TODO: Since render targets each have their own logical device, they are not device agnostic.
// TODO: Material Instantiation should be made either here or in another abstraction layer
/// <summary>
/// Abstraction layer representing a window handle and linking it to the render backend
/// <summary/>
class RenderTarget
{
public:
	RenderTarget(int _framesInFlight, HWND _targetWindow, 
		vk::Instance _instance, Camera* _camera,
		RendererDeviceManager* _deviceManager);

	void Init();
	void SetRenderPass(vk::RenderPass _renderPass);
	void Render(std::vector<DrawBuffer*>& _drawBuffers);

	void Pause(bool _paused) { m_paused = _paused; }
	bool IsPaused() const { return m_paused; }

#pragma region Getters
	vk::Format GetFormat() const { return m_format.format; }
	vk::SurfaceKHR GetSurfaceKHR() const { return m_surfaceKHR; }
	vk::CommandPool GetCommandPool() const { return m_commandPool; }

	vk::Device GetDevice() const { return m_deviceData.Device; }
	vk::PhysicalDevice GetPhysicalDevice() const { return m_deviceData.PhysicalDevice; }
	DeviceData GetDeviceData() const { return m_deviceData; }
	vk::DescriptorSetLayout GetUBODescriptorSetLayout() const { return m_uboDescriptorSetLayout; }
	vk::DescriptorSet* GetDescriptorSet() { return &m_descriptorSets[0]; }
	vk::RenderPass GetRenderPass() const { return m_renderPass; }
	uint16_t GetSubpassIndexByName(const std::string& _name) const;
	std::vector<SubpassInfo> GetSubpassInfos() const { return m_subpassInfos; }
#pragma endregion //Getters

private:
	size_t m_framesInFlight = 0;
	int m_currentFrame = 0;

	HWND m_targetWindow = nullptr;
	Material* m_defaultMaterial = nullptr;

	vk::Instance m_instance;
	DeviceData m_deviceData;

	RendererDeviceManager* m_deviceManager = nullptr;
	Camera* m_camera = nullptr;
	bool m_paused = false;

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
	std::vector<SubpassInfo> m_subpassInfos;
	std::map<std::string, uint32_t> m_subpassNameToIndexMap;
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

	void CreateDescriptorSetLayout();
	void CreateDescriptorPool();
	void CreateDescriptorSets();
#pragma endregion //Uniform Buffers Methods

	void CreateSyncObjects();

	void AddSubpass(std::string name);

	void CreateDepthSubpass(vk::AttachmentReference* _inDepthAttPtr, vk::SubpassDescription& _outDesc, vk::SubpassDependency& _outDependency);
	void CreateColorSubpass(vk::AttachmentReference* _inDepthAttPtr, vk::AttachmentReference* _inColorAttPtr,
		int _colorAttCount, vk::SubpassDescription& _outDesc, vk::SubpassDependency& _outDependency);

	void CreateRenderPass();

	void CreateCommandPool();
	void CreateCommandBuffers();
	void RecordCommandBuffer(vk::CommandBuffer& _buffer, std::vector<DrawBuffer*>& _drawBuffers);
};