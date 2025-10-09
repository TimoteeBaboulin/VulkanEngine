#pragma once

#include "common.h"

#include <memory>

#include "Material.h"
#include "ResourceManagement/TextureData.h"

//This should make the vertex and index buffer sizes be 32 mb
constexpr int MaxVertexCount = 100000;
constexpr int MaxModelCount = 500;
constexpr int MaxIndexCount = 1600000;

struct MeshEntry
{
	MeshData* Data;
	std::vector<glm::mat4x4> ModelMatrices;
	std::vector<int> TextureIndexes;
};

class BufferDeviceLink;
class RenderTarget;

//TODO: Draw buffers should not be device specific for multiple render targets
//TODO: Need to handle buffers in render targets
//TODO: Or allow draw buffers to link to multiple devices
/// <summary>
/// Class used to batch draw calls together into a single one
/// </summary>
class DrawBuffer
{
public:
	//DrawBuffer(Material* _material, Renderer& _renderer);
	DrawBuffer(Material* _material);
	bool TryAddMesh(MeshInstance* _instance);
	void RemoveMesh(MeshInstance* _instance);

	void LinkTarget(RenderTarget& _renderTarget);

	//int RemainingVertexPlaces();
	//bool MeshCanFit(MeshData _mesh);
	//bool TryAddMesh(MeshData* _meshData, glm::mat4x4 _modelMatrice, std::vector<std::shared_ptr<Image>> m_textures);

	void UpdateBuffers();
	void RenderBuffer(RenderTarget& _target, vk::CommandBuffer& _cmd, int _renderPass);
	//void RenderBufferIndirect(vk::CommandBuffer _cmd, vk::DescriptorSet* _uboSet, int _currentPass);

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
	std::vector<MeshInstance*> m_meshInstances;
	std::vector<std::shared_ptr<Image>> m_textures;
	std::vector<BufferDeviceLink> m_deviceLinks;
	std::vector<std::shared_ptr<Image>> m_textureList;

	//vk::DescriptorPool m_descriptorPool;
	//Material* m_material;

	//Raw Data (used to memcpy to the GPU effectively
	Vertex* m_vertexData;
	uint16_t* m_indexData;
	glm::mat4x4* m_modelData;

	//Vertex Data
	int m_vertexCount;
	int m_indexCount;
	int m_instanceCount;

	//Mesh Entries
	std::vector<MeshEntry> m_meshes;

	//std::vector<MeshData*> m_meshes;
	std::vector<std::vector<glm::mat4x4>> m_modelMatrices;
	std::vector<int> m_meshInstanceCount;

	//Textures
	std::vector<std::shared_ptr<Image>> m_images;
	
	Material* m_material;

	//Dirty tracking booleans
	bool m_buffersGenerated;
	bool m_texturesDirty;
	bool m_dirty;

	std::vector<vk::DrawIndexedIndirectCommand> m_drawCommands;

	void CountVertexData();
	void UpdateEntries();

	/// <summary>
	/// Function meant to add textures that aren't already there to the textureArray
	/// It returns the indexes of the textures in the texture array, whether they were added or not
	/// </summary>
	std::vector<int> AddTextures(std::vector<std::shared_ptr<Image>>& _images);
	std::vector<BufferDeviceLink>::iterator FindDeviceLink(RenderTarget& _target);

	//void UpdateTextures();
	//TextureData GetTextureData(Image _image);
	//void AddTextures(std::vector<std::shared_ptr<Image>> _images);
	//void AddDevice(vk::Device _device, vk::PhysicalDevice _physDevice);
	//void GenerateCommandPool(int _deviceIndex);
	//void GenerateDeviceResources(int _deviceIndex);
};