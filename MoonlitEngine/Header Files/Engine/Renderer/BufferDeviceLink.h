#pragma once

#include <vulkan/vulkan.hpp>
#include "common.h"
#include "ResourceManagement/TextureData.h"

class RendererDeviceManager;
class MaterialInstance;

struct MeshEntry;
struct DeviceData;

/// <summary>
/// Buffers and memory addresses for vertex/index/texture data
/// </summary>
struct DrawBufferResources
{
	vk::Buffer vertexBuffer;
	vk::Buffer indexBuffer;
	vk::Buffer modelMatrixBuffer;

	vk::DeviceMemory vertexBufferMemory;
	vk::DeviceMemory indexBufferMemory;
	vk::DeviceMemory modelMatrixBufferMemory;

	std::vector<vk::DescriptorSet> textureDescriptorSets;
	std::vector<TextureData> textures;
};

/// <summary>
/// Class interfacing between the batching system and the render targets
/// </summary>
class MOONLIT_API BufferDeviceLink
{
public:
	//TODO: Use it and pass it to private if possible
	bool IsDirty = false;

	BufferDeviceLink(DeviceData _deviceData, MaterialInstance* _materialInstance);
	~BufferDeviceLink();
	DrawBufferResources const GetBufferResources() { return m_drawResources; };
	vk::Device const GetDevice() { return m_deviceData.Device; }

	void Render(vk::CommandBuffer& _cmd, int _renderPass,
		std::vector<MeshEntry>& _meshEntries, vk::DescriptorSet* _uboSet);
	// This function has way too many parameters, need to refactor it
	// Maybe pass a struct with all the data?
	//TODO: Refactor this function
	void GenerateBuffers(Vertex* _vertexData, uint32_t _vertexCount,
		uint16_t* _indexData, uint32_t _indexCount,
		glm::mat4x4* _modelData, uint32_t _modelCount,
		int* _textureIndices);

	// Textures are handled separately as they require a different approach
	// And aren't stored in vk::Buffer structures

	void GenerateTextures(std::vector<std::shared_ptr<Image>>& _textures);

private:
	MaterialInstance* m_material;

	DrawBufferResources m_drawResources;
	DeviceData m_deviceData;
	vk::CommandPool m_commandPool;
	vk::CommandBuffer m_commandBuffer;

	std::vector<std::shared_ptr<Image>> m_loadedImages;

	bool m_resourcesGenerated = false;

	void ClearBuffers();
	void ClearTextures();
	TextureData GetTextureData(Image& _image);
	void UpdateTextures();
};