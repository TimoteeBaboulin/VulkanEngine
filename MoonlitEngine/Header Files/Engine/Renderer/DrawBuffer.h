#pragma once
#include <memory>

#include "BufferDeviceLink.h"
#include "common.h"
#include "Material/Material.h"

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

class RenderTarget;

/// <summary>
/// Class used to batch draw calls together into a single one
/// </summary>
class DrawBuffer
{
public:
	DrawBuffer(Material* _material);

	//DATA MANAGEMENT----------------------------------------------------------------------

	bool TryAddMesh(MeshInstance* _instance);
	void RemoveMesh(MeshInstance* _instance);

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
	
	std::vector<MeshInstance*> m_meshInstances;
	std::vector<BufferDeviceLink> m_deviceLinks;
	std::vector<std::shared_ptr<Image>> m_textureList;

	//Mesh Entries
	
	std::vector<MeshEntry> m_meshes;
	std::vector<std::vector<glm::mat4x4>> m_modelMatrices;
	std::vector<int> m_meshInstanceCount;

	//RAW DATA-----------------------------------------------------------------------------
	
	Vertex* m_vertexData;
	uint16_t* m_indexData;
	glm::mat4x4* m_modelData;
	int* m_textureIndices;

	//DATA COUNTS--------------------------------------------------------------------------
	
	int m_vertexCount;
	int m_indexCount;
	int m_instanceCount;
	
	Material* m_material;

	//TODO: Re-implement indirect drawing
	//std::vector<vk::DrawIndexedIndirectCommand> m_drawCommands;

	void CountVertexData();
	void UpdateEntries();

	/// <summary>
	/// Function meant to add textures that aren't already there to the textureArray<para/>
	/// It returns the indexes of the textures in the texture array, whether they were added or not
	/// </summary>
	std::vector<int> AddTextures(std::vector<std::shared_ptr<Image>>& _images);
	std::vector<BufferDeviceLink>::iterator FindDeviceLink(RenderTarget& _target);
};