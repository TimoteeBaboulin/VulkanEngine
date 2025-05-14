#include "Renderer/DrawBuffer.h"
#include "Renderer/VulkanHelperFunctions.h"

DrawBuffer::DrawBuffer()
{
	m_vertexData = new Vertex[MaxVertexCount];
	m_indexData = new uint16_t[MaxIndexCount];

	m_buffersGenerated = false;
	m_dirty = false;

	m_currentVertex = 0;
	m_currentIndex = 0;
}

int DrawBuffer::RemainingVertexPlaces()
{
	return (MaxVertexCount - m_currentVertex);
}

bool DrawBuffer::MeshCanFit(MeshData _mesh)
{
	return (_mesh.vertexCount + m_currentVertex <= MaxVertexCount) && (_mesh.triangleCount * 3 + m_currentIndex <= MaxIndexCount);
}

bool DrawBuffer::TryAddMesh(MeshData _mesh)
{
	int vertexCount = _mesh.vertexCount;

	if (!MeshCanFit(_mesh))
		throw new std::exception("Not enough place in DrawBuffer");

	Vertex* vertexBuffer = m_vertexData + m_currentVertex;
	uint16_t* indexBuffer = m_indexData + m_currentIndex;
	
	memcpy(vertexBuffer, _mesh.vertices, sizeof(Vertex) * vertexCount);
	memcpy(indexBuffer, _mesh.indices, 16 * 3 * _mesh.triangleCount);

	m_currentVertex += vertexCount;
	m_currentIndex += _mesh.triangleCount * 3;

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
		//device.freeMemory(m_vertexMemory);
		//device.freeMemory(m_indexMemory);
	}

	BufferCreateInfo info =
	{
		.buffer = m_vertexBuffer,
		.memory = m_vertexMemory,
		.usage = vk::BufferUsageFlagBits::eVertexBuffer,
		.properties = vk::MemoryPropertyFlagBits::eDeviceLocal,
		.size = sizeof(Vertex) * m_currentVertex
	};

	vhf::CreateBufferWithStaging(info, m_vertexData);

	BufferCreateInfo indexInfo =
	{
		.buffer = m_indexBuffer,
		.memory = m_indexMemory,
		.usage = vk::BufferUsageFlagBits::eIndexBuffer,
		.size = (uint64_t) 32 * m_currentIndex
	};

	vhf::CreateBufferWithStaging(indexInfo, m_indexData);

	m_buffersGenerated = true;
	m_dirty = false;
}

void DrawBuffer::RenderBuffer(vk::CommandBuffer _cmd)
{
	if (!m_buffersGenerated || m_dirty)
		return;

	vk::DeviceSize offsets[] = { 0 };
	_cmd.bindVertexBuffers(0, m_vertexBuffer, offsets);
	_cmd.bindIndexBuffer(m_indexBuffer, 0, vk::IndexType::eUint16);

	_cmd.drawIndexed(m_currentIndex, 1, 0, 0, 0);
}

