#pragma once

#include "common.h"

//This should make the vertex and index buffer sizes be 32 mb
constexpr int MaxVertexCount = 100000;
constexpr int MaxModelCount = 500;
constexpr int MaxIndexCount = 1600000;

class DrawBuffer
{
public:
	DrawBuffer();

	int RemainingVertexPlaces();
	bool MeshCanFit(MeshData _mesh);
	bool TryAddMesh(MeshData* _mesh, glm::mat4x4 _modelMatrice);

	void GenerateBuffers();
	void RenderBuffer(vk::CommandBuffer _cmd);

private:
	Vertex* m_vertexData;
	glm::mat4x4* m_modelData;
	uint16_t* m_indexData;

	int m_vertexCount;
	int m_indexCount;
	int m_instanceCount;

	std::vector<MeshData*> m_meshes;
	std::vector<std::vector<glm::mat4x4>> m_modelMatrices;
	std::vector<int> m_meshInstanceCount;

	bool m_buffersGenerated;
	bool m_dirty;

	vk::Buffer m_vertexBuffer;
	vk::Buffer m_modelMatriceBuffer;
	vk::Buffer m_indexBuffer;

	vk::DeviceMemory m_vertexMemory;
	vk::DeviceMemory m_indexMemory;
	vk::DeviceMemory m_modelMatriceMemory;

	void GenerateModelMatriceBuffer();
};