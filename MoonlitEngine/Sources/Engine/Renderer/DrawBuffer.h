#pragma once
#include <memory>

#include "BufferDeviceLink.h"
#include "../../common.h"
#include "Material/Material.h"

namespace Moonlit::Renderer
{

	//This should make the vertex and index buffer sizes be 32 mb
	constexpr int MaxVertexCount = 100000;
	constexpr int MaxModelCount = 500;
	constexpr int MaxIndexCount = 1600000;

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

	struct TextureSlot
	{
		intptr_t TextureHandle;
		uint16_t InstanceCount;
		std::shared_ptr<Image> Texture;
	};

	// Memory layout:
	// Every instance of a mesh will be in a row
	// The order in which those batches are written in are the same as the order of the meshdata in the vector
	// So to get to instance 2 of mesh 3, we skip the combined instances of meshes 0, 1 and 2 and skip the 2 instances of mesh 3 before it

	class RenderTarget;

	// This should be fixed to avoid confusion

	/// <summary>
	/// Class used to batch draw calls together into a single one
	/// </summary>
	class DrawBuffer
	{
	public:
		DrawBuffer(Material* _material, DeviceData _deviceData);
		~DrawBuffer();

		//DATA MANAGEMENT----------------------------------------------------------------------

		void RemoveMeshInstance(uint32_t _instanceId);
		uint32_t AddMeshInstance(std::shared_ptr<MeshData> _mesh,
			std::vector<std::shared_ptr<Image>> _textures,
			glm::mat4x4 _model);
		void UpdateInstanceModel(uint32_t _instanceId, glm::mat4x4 _model);
		void UpdateInstanceMesh(uint32_t _instanceId, std::shared_ptr<MeshData> _mesh);

		//RENDER-------------------------------------------------------------------------------

		void RenderBuffer(RenderTarget& _target, vk::CommandBuffer& _cmd, std::string _renderPass);

		//GETTERS------------------------------------------------------------------------------

		uint32_t GetVertexCount() const { return m_vertexCount; }
		uint32_t GetTriangleCount() const { return m_indexCount / 3; }

		std::vector<Vertex> GetVertexData() const { return m_vertexData; }
		std::vector<uint16_t> GetIndexData() const { return m_indexData; }
		std::vector<glm::mat4x4> GetModelData() const { return m_modelData; }
		std::vector<uint32_t> GetTextureIndices() const { return m_textureIndices; }

		std::vector<MeshEntry> GetMeshEntries() const { return m_meshEntries; }
		std::vector<std::shared_ptr<Image>> GetAllTextures() const;

	private:
		//DATA---------------------------------------------------------------------------------

		DrawBufferDeviceBridge m_deviceBridge;
		Material* m_material;

		////Mesh Entries

		std::vector<MeshEntry> m_meshEntries;
		std::vector<InstanceData> m_meshInstances;
		std::vector<TextureSlot> m_textureList;

		//RAW DATA-----------------------------------------------------------------------------

		// Buffer layout:
		// [Vertex Data1][Vertex Data2][Vertex Data3]...[Final VertexData][Model Matrix1][Texture Indices1][Model Matrix2][Texture Indices2]
		// Vertex data is per vertex, model matrix and texture indices are per instance
		// Index buffer is its own buffer

		std::vector<Vertex> m_vertexData;
		std::vector<uint16_t> m_indexData;
		std::vector<glm::mat4x4> m_modelData;
		std::vector<uint32_t> m_textureIndices;

		//COUNTERS--------------------------------------------------------------------------

		uint32_t m_vertexCount;
		uint32_t m_indexCount;
		uint32_t m_instanceCount;

		//TODO: Re-implement indirect drawing
		//std::vector<vk::DrawIndexedIndirectCommand> m_drawCommands;

		// MESH MANAGEMENT---------------------------------------------------------------------

		void InsertMesh(std::shared_ptr<MeshData> _mesh);
		void RemoveMeshInstance(std::vector<InstanceData>::iterator _instanceIt);
		void InsertMeshInstance(intptr_t _meshHandle, glm::mat4x4 _model, uint32_t _instanceId);
		void RemoveMesh(std::vector<MeshEntry>::iterator _instanceIt);
		std::vector<MeshEntry>::iterator FindMeshEntry(intptr_t _handle);

		// TEXTURE MANAGEMENT------------------------------------------------------------------

		std::vector<uint16_t> GetTextureIndices(std::vector<std::shared_ptr<Image>>& _images);
		uint16_t InsertTexture(std::shared_ptr<Image>& _image);
		void RemoveTexture(uint16_t _index);

		std::vector<TextureSlot>::iterator FindTexture(intptr_t _texHandle);

		void SetDeviceLinkDirty();
	};
}