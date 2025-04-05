#pragma once

#include "vulkan/vulkan.hpp"
#include "Inputs/InputManager.h"
#include "common.h"
#include "VulkanData.h"

class Camera;

class VulkanContext;
class VulkanDeviceManager;

constexpr int TEXTURE_DESCRIPTOR_COUNT = 16;

class CameraInputHandler : public InputHandler
{
private:
	Camera* m_camera;
public:
	CameraInputHandler(Camera* _camera) : m_camera(_camera) {}

	void HandleMouseMoveInput(int _deltaX, int _deltaY) override;
	void HandleMouseInput(MOUSE_KEY _key, bool _keyDown) override;
	void HandleKeyboardInput(KEYBOARD_KEY _key, bool _keyDown) override;
	void HandleGamepadInput(GAMEPAD_KEY _key, bool _keyDown) override;
	void HandleGamepadAxis(GAMEPAD_KEY _key, float _x, float _y) override;
};

class VulkanRenderer
{
public:
	VulkanRenderer(vk::Extent2D _extent, std::vector<vk::Framebuffer>* _frameBuffers);
	void Init(VulkanContext* _context, VulkanDeviceManager* _deviceManager);
	void Cleanup();

	void LoadMesh(MeshData& _mesh);

	void Render(RenderInfo _renderInfo, vk::RenderPass _renderPass);

	vk::PipelineLayout GetPipelineLayout() { return m_pipelineLayout; }
	vk::RenderPass GetRenderPass() const { return m_mainRenderPass; }
private:

	CameraInputHandler* m_inputHandler;

	Material* m_baseMaterial;
	MaterialInstance* m_baseInstance;

	vk::RenderPass m_mainRenderPass;
	vk::PipelineLayout m_pipelineLayout;

	uint32_t m_framesInFlight = 3;
	uint32_t m_currentFrame = 0;

	std::vector<Camera*> m_cameras;

#pragma region SwapchainParameters
	vk::Extent2D m_extent;
	vk::SurfaceFormatKHR m_format;
	vk::PresentModeKHR m_presentMode;
	vk::SurfaceCapabilitiesKHR m_capabilities;
	vk::SurfaceKHR m_surface;

	vk::SwapchainKHR m_swapChain;
	std::vector<vk::Image> m_images;
	std::vector<vk::ImageView> m_imageViews;

	std::vector<vk::Image> m_depthImages;
	std::vector<vk::ImageView> m_depthImageViews;
	vk::DeviceMemory m_depthMemory;
#pragma endregion

#pragma region Buffers
	std::vector<vk::CommandBuffer> m_commandBuffers;
	std::vector<vk::Framebuffer>* m_frameBuffers;

	std::vector<vk::Buffer> m_uniformBuffers;
	std::vector<vk::DeviceMemory> m_uniformMemories;
	std::vector<void*> m_uniformMaps;
#pragma endregion

#pragma region Synchronization
	std::vector<vk::Semaphore> m_imageAvailable;
	std::vector<vk::Semaphore> m_renderFinished;
	std::vector<vk::Fence> m_waitForPreviousFrame;
#pragma endregion

	std::vector<Mesh> m_meshes;

	std::vector<vk::DescriptorPool> m_descriptorPools;
	std::vector<vk::DescriptorSet> m_descriptorSets;

	vk::DescriptorSetLayout m_shaderDescriptorLayout;
	vk::DescriptorSetLayout m_uboDescriptorLayout;

	void InitSyncs();

	void PickFormat(VulkanDeviceManager* _deviceManager);
	void PickPresentMode(VulkanDeviceManager* _deviceManager);
	void CreateDescriptorSetLayouts();
	void CreatePipelineLayout();


	void CreateUniformBuffers();
	void CreateDescriptorPools();
	void CreateDescriptorSets();
	void CreateRenderPasses();
	void CreateCommandBuffers();

#pragma region Swapchain
	void CreateSwapchain();
	void CreateImageViews();
	void CreateDepthImage();
	void CreateFrameBuffers();
#pragma endregion

	void UpdateUniformBuffer(void* _map, Camera* _camera);
	void BindDescriptorSets(vk::PipelineLayout& _layout, Mesh& _mesh, vk::CommandBuffer& _cmdBuffer);
	void RecordCommandBuffer(vk::CommandBuffer& _buffer, int _imageIndex, RenderInfo& _renderInfo);
};