#pragma once

#include "vulkan/vulkan.hpp"
#include "InputManager.h"
#include "common.h"

class Camera;

class CameraInputHandler : public InputHandler
{
private:
	Camera* m_camera;
public:
	CameraInputHandler(Camera* _camera) : m_camera(_camera) {}

	void HandleMouseMoveInput(int _deltaX, int _deltaY) override;
	void HandleKeyboardInput(int _key, bool _keyDown) override;
	void HandleGamepadInput(WORD _key, bool _keyDown) override;
};

class VulkanRenderer
{
public:
	VulkanRenderer(vk::Extent2D _extent, std::vector<vk::Framebuffer>* _frameBuffers);
	void Init(vk::DescriptorSetLayout _uboDescriptorLayout, vk::DescriptorSetLayout _shaderDescriptorLayout);
	void Cleanup();

	void LoadMesh(MeshData& _mesh);

	void Render(vk::SwapchainKHR _swapchain, RenderInfo _renderInfo, vk::RenderPass _renderPass);

private:

	CameraInputHandler* m_inputHandler;

	uint32_t m_framesInFlight = 3;
	uint32_t m_currentFrame = 0;

	//glm::vec3 m_cameraPos = glm::vec3(20.0f, 30.0f, 35.0f);
	std::vector<Camera*> m_cameras;

	vk::Extent2D m_extent;

	std::vector<vk::CommandBuffer> m_commandBuffers;
	std::vector<vk::Framebuffer>* m_frameBuffers;

	std::vector<vk::Semaphore> m_imageAvailable;
	std::vector<vk::Semaphore> m_renderFinished;
	std::vector<vk::Fence> m_waitForPreviousFrame;

	std::vector<Mesh> m_meshes;

	std::vector<vk::Buffer> m_uniformBuffers;
	std::vector<vk::DeviceMemory> m_uniformMemories;
	std::vector<void*> m_uniformMaps;

	std::vector<vk::DescriptorPool> m_descriptorPools;
	std::vector<vk::DescriptorSet> m_descriptorSets;
	vk::DescriptorSetLayout m_shaderDescriptorLayout;

	vk::DescriptorSetLayout m_uboDescriptorLayout;

	void InitSyncs();
	void CreateUniformBuffers();
	void CreateDescriptorPools();
	void CreateDescriptorSets(vk::DescriptorSetLayout _descriptorLayout);
	void UpdateUniformBuffer(void* _map, Camera* _camera);
	void BindDescriptorSets(vk::PipelineLayout& _layout, Mesh& _mesh, vk::CommandBuffer& _cmdBuffer);
	void CreateCommandBuffers();

	void RecordCommandBuffer(vk::CommandBuffer& _buffer, int _imageIndex, RenderInfo& _renderInfo, vk::RenderPass _renderPass);
};