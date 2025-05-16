#include "Renderer/DrawBuffer.h"
#include "Renderer/VulkanHelperFunctions.h"

DrawBuffer::DrawBuffer()
{
	m_vertexData = new Vertex[MaxVertexCount];
	m_indexData = new uint16_t[MaxIndexCount];
	m_modelData = new glm::mat4x4[MaxModelCount];

	m_buffersGenerated = false;
	m_dirty = false;

	m_vertexCount = 0;
	m_indexCount = 0;
	m_instanceCount = 0;
}

int DrawBuffer::RemainingVertexPlaces()
{
	return (MaxVertexCount - m_vertexCount);
}

bool DrawBuffer::MeshCanFit(MeshData _mesh)
{
	return (_mesh.vertexCount + m_vertexCount <= MaxVertexCount) && (_mesh.triangleCount * 3 + m_indexCount <= MaxIndexCount);
}

bool DrawBuffer::TryAddMesh(std::pair <MeshData*, MaterialInstance*> _meshInstance, glm::mat4x4 _modelMatrice)
{
	MeshData* mesh = _meshInstance.first;
	int vertexCount = mesh->vertexCount;

	if (!MeshCanFit(*mesh))
		throw new std::exception("Not enough place in DrawBuffer");

	auto it = std::find(m_meshes.begin(), m_meshes.end(), _meshInstance);
	int index = m_meshes.size();
	if (it == m_meshes.end())
	{
		Vertex* vertexBuffer = m_vertexData + m_vertexCount;
		uint16_t* indexBuffer = m_indexData + m_indexCount;

		int indexCount = 3 * mesh->triangleCount;

		memcpy(vertexBuffer, mesh->vertices, sizeof(Vertex) * vertexCount);

		for (int i = 0; i < indexCount; i++)
		{
			indexBuffer[i] = mesh->indices[i] + m_vertexCount;
		}
		//memcpy(indexBuffer, _mesh->indices, 16 * 3 * _mesh->triangleCount);

		m_vertexCount += vertexCount;
		m_indexCount += mesh->triangleCount * 3;

		m_meshes.push_back(_meshInstance);

		m_modelMatrices.push_back(std::vector<glm::mat4x4>());
		m_meshInstanceCount.push_back(0);
	}
	else
	{
		index = std::distance(m_meshes.begin(), it);
	}

	m_modelMatrices[index].push_back(_modelMatrice);
	m_meshInstanceCount[index]++;
	m_instanceCount++;

	GenerateModelMatriceBuffer();

	m_dirty = true;

	return true;
}

void DrawBuffer::GenerateBuffers()
{
	if (!m_dirty)
		return;

	if (m_buffersGenerated)
	{
		vk::Device device = VulkanEngine::LogicalDevice;

		device.freeMemory(m_vertexMemory);
		device.freeMemory(m_indexMemory);
		device.freeMemory(m_modelMatriceMemory);
	}

	BufferCreateInfo info =
	{
		.buffer = m_vertexBuffer,
		.memory = m_vertexMemory,
		.usage = vk::BufferUsageFlagBits::eVertexBuffer,
		.properties = vk::MemoryPropertyFlagBits::eDeviceLocal,
		.size = sizeof(Vertex) * m_vertexCount
	};

	vhf::CreateBufferWithStaging(info, m_vertexData);

	BufferCreateInfo indexInfo =
	{
		.buffer = m_indexBuffer,
		.memory = m_indexMemory,
		.usage = vk::BufferUsageFlagBits::eIndexBuffer,
		.size = (uint64_t) sizeof(uint16_t) * m_indexCount
	};

	vhf::CreateBufferWithStaging(indexInfo, m_indexData);

	BufferCreateInfo modelBufferInfo =
	{
		.buffer = m_modelMatriceBuffer,
		.memory = m_modelMatriceMemory,
		.usage = vk::BufferUsageFlagBits::eVertexBuffer,
		.size = (uint64_t)64 * m_instanceCount
	};

	vhf::CreateBufferWithStaging(modelBufferInfo, m_modelData);

	m_buffersGenerated = true;
	m_dirty = false;
}

void DrawBuffer::RenderBuffer(vk::CommandBuffer _cmd, vk::DescriptorSet* _uboSet, int _currentPass)
{
	if (!m_buffersGenerated || m_dirty)
		return;

	vk::DeviceSize offsets[] = { 0 };
	_cmd.bindVertexBuffers(0, m_vertexBuffer, offsets);
	_cmd.bindVertexBuffers(1, m_modelMatriceBuffer, offsets);
	_cmd.bindIndexBuffer(m_indexBuffer, 0, vk::IndexType::eUint16);

	int currIndex = 0;
	int currInstance = 0;

	for (int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i].second->RecordCommandBuffer(_cmd, _currentPass, vk::PipelineBindPoint::eGraphics, _uboSet);

		int instanceCount = m_meshInstanceCount[i];
		int indexCount = m_meshes[i].first->triangleCount * 3;

		_cmd.drawIndexed(indexCount, 1, currIndex, 0, currInstance);

		currInstance += instanceCount;
		currIndex += indexCount;
	}
}

void DrawBuffer::GenerateModelMatriceBuffer()
{
	int index = 0;

	for (auto meshIt = m_modelMatrices.begin(); meshIt != m_modelMatrices.end(); meshIt++)
	{
		for (auto it = (*meshIt).begin(); it != (*meshIt).end(); it++)
		{
			m_modelData[index] = (*it);

			index++;
		}
	}
}

