#pragma once

#include "common.h"
#include "CustomVulkanStructs.h"
#include "ResourceManagement/TextureData.h"
#include "Material/Material.h"

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
/// This class owns the GPU resources for a DrawBuffer
/// It handles the creation, updating, and destruction of these resources
/// </summary>
class MOONLIT_API DrawBufferDeviceBridge
{
public:
	DrawBufferDeviceBridge(DeviceData _deviceData, Material& _baseMaterial,
		class DrawBuffer* _buffer);

	// Forbid copy, explicitly implement move
	// (Don't forget rule of five)

	DrawBufferDeviceBridge(DrawBufferDeviceBridge const&) = delete;
	DrawBufferDeviceBridge& operator=(DrawBufferDeviceBridge const&) = delete;
	DrawBufferDeviceBridge(DrawBufferDeviceBridge&& _src);
	DrawBufferDeviceBridge& operator=(DrawBufferDeviceBridge&& _rhs);

	~DrawBufferDeviceBridge();
	DrawBufferResources const GetBufferResources() { return m_drawResources; };
	vk::Device const GetDevice() { return m_deviceData.Device; }

	void Render(vk::CommandBuffer& _cmd, std::string _renderPass,
		vk::DescriptorSet* _uboSet);

	void SetDirty();

private:
	bool m_isDirty = false;

	std::shared_ptr<MaterialInstance> m_materialInstance;

	class DrawBuffer* m_parentBuffer;

	DrawBufferResources m_drawResources;
	DeviceData m_deviceData;
	vk::CommandPool m_commandPool;
	vk::CommandBuffer m_commandBuffer;

	std::vector<std::shared_ptr<Image>> m_loadedImages;

	bool m_resourcesGenerated = false;

	// Helper function to create a TextureData from an Image
	TextureData GetTextureData(Image& _image);

	// This is used if the buffer is dirty to manually update resources
	void UpdateData();
	void UpdateBuffers();
	void UpdateTextures();
	void UpdateTextureSets();

	// Memory management
	void AllocateCommandBuffer();
	void AllocateTextureSets();

	void GenerateBuffers();
	void GenerateTextures(std::vector<std::shared_ptr<Image>>& _textures);

	void ClearBuffers();
	void ClearTextures();
};