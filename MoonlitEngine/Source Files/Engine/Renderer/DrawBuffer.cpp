#include "Engine/Renderer/DrawBuffer.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/BufferDeviceLink.h"
#include "Engine/Renderer/RenderTarget.h"

#include "Debug/Logger.h"

DrawBuffer::DrawBuffer(Material* _material)
{
	m_material = _material;

	m_vertexData = new Vertex[MaxVertexCount];
	m_indexData = new uint16_t[MaxIndexCount];
	m_modelData = new glm::mat4x4[MaxModelCount];

	//TODO: This assumes each mesh instance has up to 4 textures
	//TODO: Update this when material can read the texture requirements of the shader
	m_textureIndices = new int[MaxModelCount * 4];

	m_vertexCount = 0;
	m_indexCount = 0;
	m_instanceCount = 0;
}

DrawBuffer::~DrawBuffer()
{
	delete[] m_vertexData;
	delete[] m_indexData;
	delete[] m_modelData;
	delete[] m_textureIndices;

	m_deviceLinks.clear();
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

	m_modelData.erase(m_modelData.begin() + index);
	m_meshInstances.erase(m_meshInstances.begin() + index);
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
	m_meshInstances.insert(m_meshInstances.begin() + instanceToSkip, instanceData);
	m_modelData.insert(m_modelData.begin() + instanceToSkip, _model);

	return instanceData.Id;
}

//bool DrawBuffer::TryAddMesh(MeshInstance* _instance)
//{
//	if (std::find(m_meshInstances.begin(), m_meshInstances.end(), _instance) != m_meshInstances.end())
//	{
//		LOG_WARNING("Trying to load already existing mesh");
//		return false;
//	}
//
//	m_meshInstances.push_back(_instance);
//
//	for (auto dit = m_deviceLinks.begin(); dit != m_deviceLinks.end(); dit++)
//	{
//		(*dit).IsDirty = true;
//	}
//
//	//Need to check whether there is already a mesh entry for this mesh data
//	bool entryFound = false;
//	auto it = m_meshes.end();
//
//	for (auto entIt = m_meshes.begin(); entIt != m_meshes.end(); entIt++)
//	{
//		if ((*entIt).Data == _instance->MeshData)
//		{
//			(*entIt).ModelMatrices.push_back(*_instance->Model);
//			entryFound = true;
//			it = entIt;
//			break;
//		}
//	}
//
//	if (!entryFound)
//	{
//		m_meshes.push_back(MeshEntry{
//			_instance->MeshData,
//			{*_instance->Model}
//			});
//
//		it = m_meshes.end() - 1;
//	}
//
//	std::vector<int> textureIndexes = AddTextures(_instance->Textures);
//	(*it).TextureIndexes.insert((*it).TextureIndexes.end(), textureIndexes.begin(), textureIndexes.end());
//
//	// This has been commented since update entries clears everything and re-adds everything
//	// We are adding things dynamically to avoid unneeded overhead
//	// UpdateEntries should be optimized to only take care of the changed data
//	// Currently it is only used when removing a mesh instance as there is no easy way
//	// To update the buffers by only removing the instance
//	//UpdateEntries();
//
//	UpdateBuffers();
//
//	return true;
//}

//void DrawBuffer::RemoveMesh(MeshInstance* _instance)
//{
//	auto it = std::find(m_meshInstances.begin(), m_meshInstances.end(), _instance);
//	if (it != m_meshInstances.end())
//		m_meshInstances.erase(it);
//
//	for (auto dit = m_deviceLinks.begin(); dit != m_deviceLinks.end(); dit++)
//	{
//		(*dit).IsDirty = true;
//	}
//
//	UpdateEntries();
//	UpdateBuffers();
//}

void DrawBuffer::LinkTarget(RenderTarget& _renderTarget)
{
	auto it = FindDeviceLink(_renderTarget);
	if (it != m_deviceLinks.end())
	{
		LOG_WARNING("Trying to link a target that's already subscribed");
		return;
	}

	m_deviceLinks.emplace_back(_renderTarget.GetDeviceData(), m_material->CreateInstance(_renderTarget));
	m_deviceLinks.back().GenerateBuffers(m_vertexData, m_vertexCount,
		m_indexData, m_indexCount,
		m_modelData, m_instanceCount, m_textureIndices);
	m_deviceLinks.back().GenerateTextures(m_textureList);
}

void DrawBuffer::UpdateBuffers()
{
	CountVertexData();

	Vertex* vertexDataPtr = m_vertexData;
	uint16_t* indexDataPtr = m_indexData;
	glm::mat4x4* modelDataPtr = m_modelData;
	int* textureIndexPtr = m_textureIndices;

	//Go over all the entries as they are meant to give an easy way to write the data as it comes
	for (auto it = m_meshes.begin(); it != m_meshes.end(); it++)
	{
		MeshData* data = (*it).Data;
		memcpy(vertexDataPtr, data->vertices, sizeof(Vertex) * data->vertexCount);
		vertexDataPtr = vertexDataPtr + data->vertexCount;

		memcpy(indexDataPtr, data->indices, sizeof(uint16_t) * data->triangleCount * 3);
		indexDataPtr = indexDataPtr + (data->triangleCount * 3);

		for (auto mit = (*it).ModelMatrices.begin(); mit != (*it).ModelMatrices.end(); mit++)
		{
			*modelDataPtr = (*mit);
			modelDataPtr++;
		}

		memcpy(textureIndexPtr, (*it).TextureIndexes.data(), sizeof(int) * (*it).TextureIndexes.size());
		textureIndexPtr = textureIndexPtr + (*it).TextureIndexes.size();
	}
}

void DrawBuffer::RenderBuffer(RenderTarget& _target, vk::CommandBuffer& _cmd, int _renderPass)
{
	if (m_meshes.size() == 0)
	{
		return;
	}

	auto it = FindDeviceLink(_target);

	if (it == m_deviceLinks.end())
	{
		return;
	}

	//Debug: force update every frame
	//Debug: This cause absolutely awful performance, but it is here to test updating the transforms
	UpdateEntries();
	UpdateBuffers();
	(*it).IsDirty = true;

	if ((*it).IsDirty)
	{
		(*it).GenerateBuffers(m_vertexData, m_vertexCount,
			m_indexData, m_indexCount,
			m_modelData, m_instanceCount, m_textureIndices);
		(*it).GenerateTextures(m_textureList);
	}

	(*it).Render(_cmd, _renderPass, m_meshes, _target.GetDescriptorSet());
}

void DrawBuffer::CountVertexData()
{
	m_vertexCount = 0;
	m_indexCount = 0;
	m_instanceCount = 0;

	for (auto it = m_meshes.begin(); it != m_meshes.end(); it++)
	{
		MeshData* data = (*it).Data;

		int instanceCount = (*it).ModelMatrices.size();
		m_instanceCount += instanceCount;
		m_vertexCount += data->vertexCount;
		m_indexCount += data->triangleCount * 3;
	}
}

void DrawBuffer::UpdateEntries()
{
	for (auto& entry: m_meshes)
	{
		entry.ModelMatrices.clear();
	}


	for (int index = 0; index < m_meshInstances.size(); index++)
	{
		MeshInstance& instance = *m_meshInstances[index];
		MeshData* meshData = instance.MeshData;
		auto meshEntryIt = m_meshes.end();

		for (auto it = m_meshes.begin(); it != m_meshes.end(); it++)
		{
			if ((*it).Data == meshData)
			{
				meshEntryIt = it;
				break;
			}
		}

		//TODO: Add checks to know if there is enough place in the buffer to add the mesh instance

		if (meshEntryIt == m_meshes.end())
		{
			m_meshes.push_back(MeshEntry{
				meshData,
				{*instance.Model}
				});

			continue;
		}

		(*meshEntryIt).ModelMatrices.push_back(*instance.Model);
	}
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
		m_vertexData.emplace_back((*_mesh).indices[i] + baseIndex);
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

void DrawBuffer::RemoveMesh(std::vector<MeshEntry>::iterator _instanceIt)
{
	//TODO: add mesh removal
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

std::vector<int> DrawBuffer::AddTextures(std::vector<std::shared_ptr<Image>>& _images)
{
	std::vector<int> textureIndexes;

	for (size_t i = 0; i < _images.size(); i++)
	{
		auto imageIt = std::find(m_textureList.begin(), m_textureList.end(), _images[i]);
		int index = -1;
		if (imageIt == m_textureList.end())
		{
			//TODO: Add a check that we aren't exceeding the material's texture array size
			index = m_textureList.size();
			m_textureList.push_back(_images[i]);
			textureIndexes.push_back(index);
		}
		else
		{
			index = std::distance(m_textureList.begin(), imageIt);
			textureIndexes.push_back(index);
		}
	}

	return textureIndexes;
}

std::vector<BufferDeviceLink>::iterator DrawBuffer::FindDeviceLink(RenderTarget& _target)
{
	for (auto it = m_deviceLinks.begin(); it != m_deviceLinks.end(); it++)
	{
		if ((*it).GetDevice() == _target.GetDevice())
		{
			return it;
		}
	}

	return m_deviceLinks.end();
}