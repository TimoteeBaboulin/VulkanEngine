#pragma once

#include "common.h"

#include "Material.h"
#include "ResourceManagement/TextureData.h"

//This should make the vertex and index buffer sizes be 32 mb
constexpr int MaxVertexCount = 100000;
constexpr int MaxModelCount = 500;
constexpr int MaxIndexCount = 1600000;

struct DeviceDrawResources
{
	vk::Buffer vertexBuffer;
	vk::DeviceMemory vertexMemory;

	vk::Buffer indexBuffer;
	vk::DeviceMemory indexMemory;

	vk::Buffer modelMatriceBuffer;
	vk::DeviceMemory modelMatriceMemory;

	vk::Buffer drawCommandBuffer;
	vk::DeviceMemory drawCommandMemory;

	std::vector<TextureData> textures;
	std::vector<int> textureIndexes;
	std::vector<vk::DescriptorSet> descriptorSets;

	bool buffersGenerated = false;
};

//TODO: Draw buffers should not be device specific for multiple render targets
//TODO: Need to handle buffers in render targets
//TODO: Or allow draw buffers to link to multiple devices
/// <summary>
/// Class used to batch draw calls together into a single one
/// </summary>
class DrawBuffer
{
public:
	DrawBuffer(Material* _material, Renderer& _renderer);

	int RemainingVertexPlaces();
	bool MeshCanFit(MeshData _mesh);
	bool TryAddMesh(MeshData* _meshData, glm::mat4x4 _modelMatrice, std::vector<std::shared_ptr<Image>> m_textures);

	void UpdateBuffers();
	void GenerateBuffers(int _deviceIndex);
	void RenderBuffer(vk::CommandBuffer _cmd, vk::DescriptorSet* _uboSet, int _currentPass);
	void RenderBufferIndirect(vk::CommandBuffer _cmd, vk::DescriptorSet* _uboSet, int _currentPass);

	uint32_t GetVertexCount() const { return m_vertexCount; }
	uint32_t GetTriangleCount() const 
	{ 
		int triangleCount = 0;
		int index = 0;

		for (auto it = m_meshes.begin(); it != m_meshes.end(); it++)
		{
			triangleCount += (*it)->triangleCount * m_meshInstanceCount[index];

			index++;
		}

		return triangleCount;
	}
private:

	//GPU Data
	//vk::Device m_device;
	//vk::PhysicalDevice m_physicalDevice;
	//vk::CommandPool m_mainCommandPool;
	//vk::Queue m_graphicsQueue;

	vk::DescriptorPool m_descriptorPool;
	Material* m_material;

	//Raw Data (used to memcpy to the GPU effectively
	Vertex* m_vertexData;
	uint16_t* m_indexData;
	glm::mat4x4* m_modelData;

	std::vector<vk::Device> m_linkedDevices;
	std::vector<vk::PhysicalDevice> m_linkedPhysicalDevices;
	std::vector<vk::CommandPool> m_commandPools;
	std::vector<DeviceDrawResources> m_deviceResources;

	//Vertex Data
	int m_vertexCount;
	int m_indexCount;
	int m_instanceCount;

	std::vector<MeshData*> m_meshes;
	std::vector<std::vector<glm::mat4x4>> m_modelMatrices;
	std::vector<int> m_meshInstanceCount;

	//Textures
	std::vector<std::shared_ptr<Image>> m_images;
	

	//Dirty tracking booleans
	bool m_buffersGenerated;
	bool m_texturesDirty;
	bool m_dirty;

	std::vector<vk::DrawIndexedIndirectCommand> m_drawCommands;

	void GenerateModelMatriceBuffer();
	void UpdateTextures();
	TextureData GetTextureData(Image _image);
	void AddTextures(std::vector<std::shared_ptr<Image>> _images);
	void AddDevice(vk::Device _device, vk::PhysicalDevice _physDevice);
	void GenerateCommandPool(int _deviceIndex);
	void GenerateDeviceResources(int _deviceIndex);
};