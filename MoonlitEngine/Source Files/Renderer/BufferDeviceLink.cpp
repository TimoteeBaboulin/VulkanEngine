#include "Renderer/BufferDeviceLink.h"
#include "Renderer/RendererDeviceManager.h"

BufferDeviceLink::BufferDeviceLink(DeviceData _deviceData)
{
	m_device = _deviceData.Device;
	m_physDevice = _deviceData.PhysicalDevice;

	vk::CommandPoolCreateInfo poolInfo;
	poolInfo.sType = vk::StructureType::eCommandPoolCreateInfo;
	poolInfo.queueFamilyIndex = _deviceData.QueueIndices.graphicsFamily.value();
	poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
	m_commandPool = m_device.createCommandPool(poolInfo);

	vk::CommandBufferAllocateInfo allocInfo;
	allocInfo.sType = vk::StructureType::eCommandBufferAllocateInfo;
	allocInfo.commandPool = m_commandPool;
	allocInfo.level = vk::CommandBufferLevel::ePrimary;
	allocInfo.commandBufferCount = 1;
	auto result = m_device.allocateCommandBuffers(allocInfo);
	m_commandBuffer = result.at(0);
}

BufferDeviceLink::~BufferDeviceLink()
{
	//TODO: Destroy the command pool and every resource generated on the device
}

void BufferDeviceLink::AddMesh(MeshInstance* _meshInstance)
{
	int vertexCount = _meshInstance->MeshData.vertexCount;

//	if (!MeshCanFit(*_meshInstance))
//		throw new std::exception("Not enough place in DrawBuffer");
//
//	AddTextures(_textures);
//
//	//Add the mesh to the buffer
//	auto it = std::find(m_meshes.begin(), m_meshes.end(), _meshInstance);
//	int index = m_meshes.size();
//	if (it == m_meshes.end())
//	{
//		//If it doesn't exist already we have to add the vertex and index datas
//		Vertex* vertexBuffer = m_vertexData + m_vertexCount;
//		uint16_t* indexBuffer = m_indexData + m_indexCount;
//
//		int indexCount = 3 * _meshInstance->triangleCount;
//
//		memcpy(vertexBuffer, _meshInstance->vertices, sizeof(Vertex) * vertexCount);
//
//		for (int i = 0; i < indexCount; i++)
//		{
//			indexBuffer[i] = _meshInstance->indices[i] + m_vertexCount;
//		}
//		//memcpy(indexBuffer, _mesh->indices, 16 * 3 * _mesh->triangleCount);
//
//		m_vertexCount += vertexCount;
//		m_indexCount += _meshInstance->triangleCount * 3;
//
//		m_meshes.push_back(_meshInstance);
//
//		m_modelMatrices.push_back(std::vector<glm::mat4x4>());
//		m_meshInstanceCount.push_back(0);
//	}
//	else
//	{
//		index = std::distance(m_meshes.begin(), it);
//	}
//
//	m_modelMatrices[index].push_back(_modelMatrice);
//	m_meshInstanceCount[index]++;
//	m_instanceCount++;
//
//	GenerateModelMatriceBuffer();
//
//	m_dirty = true;
//
//	return true;
}

