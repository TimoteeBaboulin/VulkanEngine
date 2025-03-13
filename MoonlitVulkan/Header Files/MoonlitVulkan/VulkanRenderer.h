#pragma once

#include "vulkan/vulkan.hpp"
#include "InputManager.h"
#include "common.h"

class CameraInputHandler : public InputHandler
{
private:
	glm::vec3* m_cameraPos;
public:
	CameraInputHandler(glm::vec3* _cameraPos) : m_cameraPos(_cameraPos) {}

	void HandleMouseMoveInput(int _deltaX, int _deltaY) override;
	void HandleKeyboardInput(int _key, bool _keyDown) override;
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

	glm::vec3 m_cameraPos = glm::vec3(20.0f, 30.0f, 35.0f);

	//Vertex* m_vertices;
	//uint32_t* m_indexes;
	//int m_vertexCount;
	//int m_triangleCount;

	vk::Extent2D m_extent;

	std::vector<vk::CommandBuffer> m_commandBuffers;
	std::vector<vk::Framebuffer>* m_frameBuffers;

	std::vector<vk::Semaphore> m_imageAvailable;
	std::vector<vk::Semaphore> m_renderFinished;
	std::vector<vk::Fence> m_waitForPreviousFrame;

	//vk::Buffer m_vertexBuffer;
	//vk::DeviceMemory m_vertexMemory;
	//vk::Buffer m_indexBuffer;
	//vk::DeviceMemory m_indexMemory;

	std::vector<Mesh> m_meshes;

	std::vector<vk::Buffer> m_uniformBuffers;
	std::vector<vk::DeviceMemory> m_uniformMemories;
	std::vector<void*> m_uniformMaps;

	std::vector<vk::DescriptorPool> m_descriptorPools;
	std::vector<vk::DescriptorSet> m_descriptorSets;
	vk::DescriptorSetLayout m_shaderDescriptorLayout;

	vk::DescriptorSetLayout m_uboDescriptorLayout;

	//RenderInfo m_renderInfo;

	void InitSyncs();
	//void CreateIndexBuffer();
	//void CreateVertexBuffers(vk::Queue _queue);
	void CreateUniformBuffers();
	void CreateDescriptorPools();
	void CreateDescriptorSets(vk::DescriptorSetLayout _descriptorLayout);
	void UpdateUniformBuffer(void* _map);
	void BindDescriptorSets(vk::PipelineLayout& _layout, Mesh& _mesh, vk::CommandBuffer& _cmdBuffer);
	//void CreateFrameBuffers();
	void CreateCommandBuffers();

	void RecordCommandBuffer(vk::CommandBuffer& _buffer, int _imageIndex, RenderInfo& _renderInfo, vk::RenderPass _renderPass);
};