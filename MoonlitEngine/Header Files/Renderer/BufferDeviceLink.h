#pragma once

#include <vulkan/vulkan.hpp>
#include "common.h"

class RendererDeviceManager;
class MaterialInstance;

struct MeshEntry;

struct DrawBufferResources
{
	vk::Buffer vertexBuffer;
	vk::Buffer indexBuffer;
	vk::Buffer modelMatrixBuffer;

	vk::DeviceMemory vertexBufferMemory;
	vk::DeviceMemory indexBufferMemory;
	vk::DeviceMemory modelMatrixBufferMemory;

	std::vector<vk::DescriptorSet> textureDescriptorSets;
	std::vector<vk::Image> textureImages;
	std::vector<vk::ImageView> textureImageViews;
	std::vector<vk::Sampler> textureSamplers;
};

class BufferDeviceLink
{
public:
	bool IsDirty = false;

	BufferDeviceLink(DeviceData _deviceData, MaterialInstance* _materialInstance);
	~BufferDeviceLink();
	DrawBufferResources& const GetBufferResources() { return m_drawResources; };
	vk::Device const GetDevice() { return m_deviceData.Device; }

	void Render(vk::CommandBuffer& _cmd, int _renderPass,
		std::vector<MeshEntry>& _meshEntries, vk::DescriptorSet* _uboSet);
	void GenerateBuffers(Vertex* _vertexData, uint32_t _vertexCount,
		uint16_t* _indexData, uint32_t _indexCount,
		glm::mat4x4* _modelData, uint32_t _modelCount);
	void GenerateTextures(std::vector<>)

private:
	MaterialInstance* m_material;

	DrawBufferResources m_drawResources;
	DeviceData m_deviceData;
	vk::CommandPool m_commandPool;
	vk::CommandBuffer m_commandBuffer;

	bool m_resourcesGenerated = false;

	void ClearBuffers();
};