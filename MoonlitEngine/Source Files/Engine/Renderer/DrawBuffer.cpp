#include "Engine/Renderer/DrawBuffer.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/BufferDeviceLink.h"
#include "Engine/Renderer/RenderTarget.h"

#include "Debug/Logger.h"

DrawBuffer::DrawBuffer(Material* _material, DeviceData _deviceData)
	: m_deviceBridge(_deviceData, *_material, this)
{
	m_material = _material;

	m_vertexCount = 0;
	m_indexCount = 0;
	m_instanceCount = 0;
}

DrawBuffer::~DrawBuffer()
{
}

void DrawBuffer::RemoveMeshInstance(uint32_t _instanceId)
{
	auto it = std::find_if(m_meshInstances.begin(), m_meshInstances.end(),
		[_instanceId](const InstanceData& _data)
		{
			return _data.Id == _instanceId;
		});

	if (it == m_meshInstances.end())
	{
		LOG_WARNING("Trying to remove mesh instance that isn't in the draw buffer.");
		return;
	}

	size_t index = std::distance(m_meshInstances.begin(), it);

	auto meshIt = FindMeshEntry((*it).MeshHandle);
	if (meshIt == m_meshEntries.end())
	{
		LOG_ERROR("Trying to remove mesh instance that exists but without a corresponding mesh entry");
		throw std::runtime_error("Mesh entry not found!");
	}

	(*meshIt).InstanceCount--;
	if ((*meshIt).InstanceCount == 0)
	{
		RemoveMesh(meshIt);
	}

	m_modelData.erase(m_modelData.begin() + (index * 2));
	m_modelData.erase(m_modelData.begin() + (index * 2));
	m_meshInstances.erase(m_meshInstances.begin() + index);

	SetDeviceLinkDirty();
}

uint32_t DrawBuffer::AddMeshInstance(std::shared_ptr<MeshData> _mesh, std::vector<std::shared_ptr<Image>> _textures, glm::mat4x4 _model)
{
	static uint32_t currentId = 0;

	intptr_t meshHandle = reinterpret_cast<intptr_t>(_mesh.get());
	auto it = FindMeshEntry(meshHandle);

	// Insert the new mesh in case it doesn't already exist
	// Vertex/Index data is updated here
	if (it == m_meshEntries.end())
	{
		InsertMesh(_mesh); 
		it = m_meshEntries.end() - 1;
	}
	
	// Since we need to find the correct place in the vector, we have to calculate to find the correct index
	InstanceData instanceData;
	instanceData.Id = currentId++;
	instanceData.MeshHandle = (*it).MeshHandle;

	size_t instanceToSkip = 0;
	for (auto entIt = m_meshEntries.begin(); entIt != it; entIt++)
	{
		instanceToSkip += (*entIt).InstanceCount;
	}

	instanceToSkip += (*it).InstanceCount;

	(*it).InstanceCount++;
	
	std::vector<uint16_t> textureIndices = GetTextureIndices(_textures);

	m_meshInstances.insert(m_meshInstances.begin() + instanceToSkip, instanceData);
	m_modelData.insert(m_modelData.begin() + (instanceToSkip * 2), _model);
	m_modelData.insert(m_modelData.begin() + (instanceToSkip * 2) + 1, glm::inverse(_model));

	m_textureIndices.insert(m_textureIndices.begin() + instanceToSkip * m_material->GetTextureCount(), 
		std::make_move_iterator(textureIndices.begin()), std::make_move_iterator(textureIndices.end()));

	SetDeviceLinkDirty();

	return instanceData.Id;
}

void DrawBuffer::UpdateInstanceModel(uint32_t _instanceId, glm::mat4x4 _model)
{
	auto it = std::find_if(m_meshInstances.begin(), m_meshInstances.end(),
		[_instanceId](const InstanceData& _data)
		{
			return _data.Id == _instanceId;
		});
	if (it == m_meshInstances.end())
	{
		LOG_WARNING("Trying to update mesh instance that isn't in the draw buffer.");
		return;
	}

	size_t index = std::distance(m_meshInstances.begin(), it);
	m_modelData[index * 2] = _model;
	m_modelData[index * 2 + 1] = glm::inverse(_model);

	SetDeviceLinkDirty();
}

void DrawBuffer::UpdateInstanceMesh(uint32_t _instanceId, std::shared_ptr<MeshData> _mesh)
{
	// Find the instance data
	auto it = std::find_if(m_meshInstances.begin(), m_meshInstances.end(),
		[_instanceId](const InstanceData& _data)
		{
			return _data.Id == _instanceId;
		});

	if (it == m_meshInstances.end())
	{
		LOG_WARNING("Trying to update mesh instance that isn't in the draw buffer.");
		return;
	}

	auto meshIt = FindMeshEntry((*it).MeshHandle);
	if (meshIt == m_meshEntries.end())
	{
		LOG_ERROR("No corresponding Mesh loaded despite the instance existing");
		throw std::runtime_error("Mesh entry not found!");
	}

	// Decrease the instance count of the old mesh
	(*meshIt).InstanceCount--;

	// Remove the old mesh if it is no longer used in order to keep the buffers clean
	if ((*meshIt).InstanceCount == 0)
	{
		RemoveMesh(meshIt);
	}

	// Get the new mesh handle and entry
	intptr_t newMeshHandle = reinterpret_cast<intptr_t>(_mesh.get());
	auto newMeshIt = FindMeshEntry(newMeshHandle);

	// Insert the new mesh in case it doesn't already exist
	if (newMeshIt == m_meshEntries.end())
	{
		InsertMesh(_mesh);
		newMeshIt = m_meshEntries.end() - 1;
	}
	(*newMeshIt).InstanceCount++;
	(*it).MeshHandle = newMeshHandle;

	SetDeviceLinkDirty();
}

void DrawBuffer::RenderBuffer(RenderTarget& _target, vk::CommandBuffer& _cmd, std::string _renderPass)
{
	if (m_meshEntries.size() == 0)
	{
		return;
	}

	std::vector<std::string > includedSubpasses = m_material->GetIncludedSubpasses();
	if (std::find(includedSubpasses.begin(), includedSubpasses.end(), _renderPass) == includedSubpasses.end())
	{
		// The material used in this draw buffer doesn't include the requested subpass
		return;
	}

	// Get the device bridge
	// Since the render targets are now only on a single logical device
	// We only have a single device bridge per draw buffer
	// As such it is now statically allocated

	m_deviceBridge.Render(_cmd, _renderPass, _target.GetDescriptorSet());
}

std::vector<std::shared_ptr<Image>> DrawBuffer::GetAllTextures() const
{
	std::vector<std::shared_ptr<Image>> textures;

	for (auto it = m_textureList.begin(); it != m_textureList.end(); it++)
	{
		textures.push_back((*it).Texture);
	}

	return textures;
}

void DrawBuffer::InsertMesh(std::shared_ptr<MeshData> _mesh)
{
	m_meshEntries.push_back(MeshEntry{
		reinterpret_cast<intptr_t>(_mesh.get()),
		0
		});

	// Load data on the stack for faster operations
	// Pre-calculate the final size to avoid causing multiple resizes
	size_t vertexCount = (*_mesh).vertexCount;
	size_t indexCount = (*_mesh).triangleCount * 3;
	size_t baseIndex = m_indexCount;
	m_vertexCount += vertexCount;
	m_indexCount += indexCount;
	m_vertexData.reserve(m_vertexCount);
	m_indexData.reserve(m_indexCount);

	for (size_t i = 0; i < vertexCount; i++)
	{
		m_vertexData.emplace_back((*_mesh).vertices[i]);
	}

	for (size_t i = 0; i < indexCount; i++)
	{
		m_indexData.emplace_back((*_mesh).indices[i] + baseIndex);
	}

	// Create a copy in case the mesh is unloaded despite still existing in the draw buffer
	MeshEntry& newEntry = m_meshEntries.back();
	newEntry.Data.vertices = new Vertex[_mesh->vertexCount];
	memcpy(newEntry.Data.vertices, _mesh->vertices, sizeof(Vertex) * _mesh->vertexCount);
	newEntry.Data.vertexCount = _mesh->vertexCount;
	newEntry.Data.indices = new uint16_t[_mesh->triangleCount * 3];
	memcpy(newEntry.Data.indices, _mesh->indices, sizeof(uint16_t) * _mesh->triangleCount * 3);
	newEntry.Data.triangleCount = _mesh->triangleCount;
}

void DrawBuffer::RemoveMesh(std::vector<MeshEntry>::iterator _meshIt)
{
	// Ensure no instances are using this mesh anymore
	assert((*_meshIt).InstanceCount == 0);

	// Remove vertex and index data from the buffers
	auto vertexStartIt = m_vertexData.begin();
	for (auto it = m_meshEntries.begin(); it != _meshIt; it++)
	{
		vertexStartIt += (*it).Data.vertexCount;
	}

	m_vertexData.erase(vertexStartIt, vertexStartIt + (*_meshIt).Data.vertexCount);

	auto indexStartIt = m_indexData.begin();
	size_t indexOffset = 0;
	for (auto it = m_meshEntries.begin(); it != _meshIt; it++)
	{
		indexStartIt += (*it).Data.triangleCount * 3;
		indexOffset += (*it).Data.triangleCount * 3;
	}
	m_indexData.erase(indexStartIt, indexStartIt + (*_meshIt).Data.triangleCount * 3);
	// Fix indices after the removed mesh
	while (indexOffset < m_indexData.size())
	{
		m_indexData[indexOffset] -= static_cast<uint16_t>((*_meshIt).Data.vertexCount);
		indexOffset++;
	}

	m_vertexCount -= (*_meshIt).Data.vertexCount;
	m_indexCount -= (*_meshIt).Data.triangleCount * 3;

	delete[](*_meshIt).Data.vertices;
	delete[](*_meshIt).Data.indices;

	m_meshEntries.erase(_meshIt);
	return;
}

std::vector<MeshEntry>::iterator DrawBuffer::FindMeshEntry(intptr_t _handle)
{
	return std::find_if(m_meshEntries.begin(), m_meshEntries.end(),
		[_handle](const MeshEntry& entry)
		{
			return entry.MeshHandle == _handle;
		});
}

std::vector<uint16_t> DrawBuffer::GetTextureIndices(std::vector<std::shared_ptr<Image>>& _images)
{
	std::vector<uint16_t> textureIndexes;

	for (auto it = _images.begin(); it != _images.end(); it++)
	{
		intptr_t texHandle = reinterpret_cast<intptr_t>((*it).get());
		auto texIt = FindTexture(texHandle);

		if (texIt == m_textureList.end())
		{
			uint16_t newIndex = InsertTexture(*it);
			textureIndexes.push_back(newIndex);
		}
		else
		{
			// Since this is after checking that the texture exists,
			// The iterator is guaranteed to be valid
			(*texIt).InstanceCount++;
			textureIndexes.push_back(static_cast<uint16_t>(std::distance(m_textureList.begin(), texIt)));
		}
	}

	return textureIndexes;
}

uint16_t DrawBuffer::InsertTexture(std::shared_ptr<Image>& _image)
{
	TextureSlot newEntry;
	newEntry.TextureHandle = reinterpret_cast<intptr_t>(_image.get());
	newEntry.InstanceCount = 1;
	newEntry.Texture = _image;

	m_textureList.emplace_back(newEntry);

	return m_textureList.size() - 1;
}

void DrawBuffer::RemoveTexture(uint16_t _index)
{
	m_textureList.erase(m_textureList.begin() + _index);
}

std::vector<TextureSlot>::iterator DrawBuffer::FindTexture(intptr_t _texHandle)
{
	return std::find_if(m_textureList.begin(), m_textureList.end(),
		[_texHandle](const TextureSlot& _entry)
		{
			return _entry.TextureHandle == _texHandle;
		});
}

void DrawBuffer::SetDeviceLinkDirty()
{
	m_deviceBridge.SetDirty();
}