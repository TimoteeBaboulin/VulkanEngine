#pragma once
#include <memory>

#include "BufferDeviceLink.h"
#include "common.h"
#include "Material/Material.h"

//This should make the vertex and index buffer sizes be 32 mb
constexpr int MaxVertexCount = 100000;
constexpr int MaxModelCount = 500;
constexpr int MaxIndexCount = 1600000;

//struct MeshEntry
//{
//	MeshData* Data;
//	std::vector<glm::mat4x4> ModelMatrices;
//	std::vector<int> TextureIndexes;
//};

struct InstanceData
{
	// This is the handle to the current entry
	// It is an auto-incremented ID assigned when adding the mesh instance to the draw buffer
	uint32_t Id;
	intptr_t MeshHandle;
};

// This structure holds the buffer data for a specific mesh
struct MeshEntry
{
	intptr_t MeshHandle;
	MeshData Data;
	size_t InstanceCount;
};

// Memory layout:
// Every instance of a mesh will be in a row
// The order in which those batches are written in are the same as the order of the meshdata in the vector
// So to get to instance 2 of mesh 3, we skip the combined instances of meshes 0, 1 and 2 and skip the 2 instances of mesh 3 before it

class RenderTarget;

/// <summary>
/// Class used to batch draw calls together into a single one
/// </summary>
class DrawBuffer
{
public:
	DrawBuffer(Material* _material);
	~DrawBuffer();

	//DATA MANAGEMENT----------------------------------------------------------------------

	void RemoveMeshInstance(uint32_t _instanceId);
	uint32_t AddMeshInstance(std::shared_ptr<MeshData> _mesh, 
		std::vector<std::shared_ptr<Image>> _textures,
		glm::mat4x4 _model);
	//bool TryAddMesh(MeshInstance* _instance);
	//void RemoveMesh(MeshInstance* _instance);

	//TODO: Add a remove target function
	
	void LinkTarget(RenderTarget& _renderTarget);
	void UpdateBuffers();

	//RENDER-------------------------------------------------------------------------------
	
	void RenderBuffer(RenderTarget& _target, vk::CommandBuffer& _cmd, int _renderPass);

	//GETTERS------------------------------------------------------------------------------
	
	uint32_t GetVertexCount() const { return m_vertexCount; }
	uint32_t GetTriangleCount() const 
	{ 
		int triangleCount = 0;
		int index = 0;

		for (auto it = m_meshes.begin(); it != m_meshes.end(); it++)
		{
			triangleCount += (int)(*it).ModelMatrices.size() * (*it).Data->triangleCount;

			index++;
		}

		return triangleCount;
	}

private:
	//DATA---------------------------------------------------------------------------------
	//
	//std::vector<MeshInstance*> m_meshInstances;
	//std::vector<BufferDeviceLink> m_deviceLinks;
	//std::vector<std::shared_ptr<Image>> m_textureList;

	////Mesh Entries
	//
	//std::vector<MeshEntry> m_meshes;
	//std::vector<std::vector<glm::mat4x4>> m_modelMatrices;
	//std::vector<int> m_meshInstanceCount;

	std::vector<MeshEntry> m_meshEntries;
	std::vector<InstanceData> m_meshInstances;

	//RAW DATA-----------------------------------------------------------------------------
	
	// Buffer layout:
	// [Vertex Data1][Vertex Data2][Vertex Data3]...[Final VertexData][Model Matrix1][Texture Indices1][Model Matrix2][Texture Indices2]
	// Vertex data is per vertex, model matrix and texture indices are per instance
	// Index buffer is its own buffer

	std::vector<Vertex> m_vertexData;
	std::vector<uint16_t> m_indexData;
	std::vector<glm::mat4x4> m_modelData;
	std::vector<int> m_textureIndices;

	//DATA COUNTS--------------------------------------------------------------------------
	
	int m_vertexCount;
	int m_indexCount;
	int m_instanceCount;
	
	Material* m_material;

	//TODO: Re-implement indirect drawing
	//std::vector<vk::DrawIndexedIndirectCommand> m_drawCommands;

	void CountVertexData();
	void UpdateEntries();

	void InsertMesh(std::shared_ptr<MeshData> _mesh);
	void RemoveMesh(std::vector<MeshEntry>::iterator _instanceIt);
	std::vector<MeshEntry>::iterator FindMeshEntry(intptr_t _handle);

	/// <summary>
	/// Function meant to add textures that aren't already there to the textureArray<para/>
	/// It returns the indexes of the textures in the texture array, whether they were added or not
	/// </summary>
	std::vector<int> AddTextures(std::vector<std::shared_ptr<Image>>& _images);
	std::vector<BufferDeviceLink>::iterator FindDeviceLink(RenderTarget& _target);
};