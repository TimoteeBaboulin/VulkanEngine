#pragma once

#include "common.h"

//This should make the vertex and index buffer sizes be 32 mb
constexpr int MaxVertexCount = 100000;
constexpr int MaxIndexCount = 1600000;

class DrawBuffer
{
public:
	DrawBuffer();

	int RemainingVertexPlaces();
	bool MeshCanFit(MeshData _mesh);
	bool TryAddMesh(MeshData _mesh);

	void GenerateBuffers();
	void RenderBuffer(vk::CommandBuffer _cmd);

private:
	Vertex* m_vertexData;
	uint16_t* m_indexData;

	int m_currentVertex;
	int m_currentIndex;

	std::vector<MeshData*> m_meshes;

	bool m_buffersGenerated;
	bool m_dirty;

	vk::Buffer m_vertexBuffer;
	vk::Buffer m_indexBuffer;

	vk::DeviceMemory m_vertexMemory;
	vk::DeviceMemory m_indexMemory;
};