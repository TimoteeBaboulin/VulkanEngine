#pragma once

#include "vulkan/vulkan.hpp"
#include "common.h"


class VulkanRenderer
{
public:
	VulkanRenderer(vk::Extent2D _extent, std::vector<vk::Framebuffer>* _frameBuffers);
	void Init(vk::DescriptorSetLayout _descriptorLayout);
	void Cleanup();

	void LoadMesh(MeshData& _mesh);

	void Render(vk::SwapchainKHR _swapchain, RenderInfo _renderInfo, vk::RenderPass _renderPass);

private:

	uint32_t m_framesInFlight = 3;
	uint32_t m_currentFrame = 0;

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
	vk::DescriptorSetLayout m_descriptorLayout;

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