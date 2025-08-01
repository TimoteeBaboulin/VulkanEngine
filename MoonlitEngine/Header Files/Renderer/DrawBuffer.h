#pragma once

#include "common.h"

#include "VulkanData.h"
#include "ResourceManagement/TextureData.h"

//This should make the vertex and index buffer sizes be 32 mb
constexpr int MaxVertexCount = 100000;
constexpr int MaxModelCount = 500;
constexpr int MaxIndexCount = 1600000;

class DrawBuffer
{
public:
	DrawBuffer(Material* _material);

	int RemainingVertexPlaces();
	bool MeshCanFit(MeshData _mesh);
	bool TryAddMesh(MeshData* _meshData, glm::mat4x4 _modelMatrice, std::vector<TextureData*> m_textures);

	void GenerateBuffers();
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
	Vertex* m_vertexData;
	uint16_t* m_indexData;

	glm::mat4x4* m_modelData;

	Material* m_material;

	int m_vertexCount;
	int m_indexCount;
	int m_instanceCount;

	std::vector<MeshData*> m_meshes;
	std::vector<std::vector<glm::mat4x4>> m_modelMatrices;
	std::vector<int> m_meshInstanceCount;

	std::vector<vk::DrawIndexedIndirectCommand> m_drawCommands;

	std::vector<TextureData*> m_textures;
	std::vector<int> m_textureIndexes;

	bool m_buffersGenerated;
	bool m_texturesDirty;
	bool m_dirty;

	vk::Buffer m_vertexBuffer;
	vk::Buffer m_modelMatriceBuffer;
	vk::Buffer m_indexBuffer;

	vk::Buffer m_drawCommandBuffer;

	vk::DeviceMemory m_vertexMemory;
	vk::DeviceMemory m_indexMemory;
	vk::DeviceMemory m_modelMatriceMemory;

	vk::DeviceMemory m_drawCommandMemory;

	std::vector<vk::DescriptorSet> m_descriptorSets;
	vk::DescriptorPool m_descriptorPool;

	void GenerateModelMatriceBuffer();
	void UpdateTextures();
};