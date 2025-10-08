#include "Renderer/BufferDeviceLink.h"
#include "Renderer/RendererDeviceManager.h"
#include "Renderer/VulkanHelperFunctions.h"
#include "Renderer/MaterialInstance.h"
#include "Renderer/DrawBuffer.h"
#include "Renderer/MaterialInstance.h"

BufferDeviceLink::BufferDeviceLink(DeviceData _deviceData, MaterialInstance* _materialInstance)
{
	m_deviceData = _deviceData;
	m_material = _materialInstance;

	vk::CommandPoolCreateInfo poolInfo;
	poolInfo.sType = vk::StructureType::eCommandPoolCreateInfo;
	poolInfo.queueFamilyIndex = _deviceData.QueueIndices.graphicsFamily.value();
	poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
	m_commandPool = m_deviceData.Device.createCommandPool(poolInfo);

	vk::CommandBufferAllocateInfo allocInfo;
	allocInfo.sType = vk::StructureType::eCommandBufferAllocateInfo;
	allocInfo.commandPool = m_commandPool;
	allocInfo.level = vk::CommandBufferLevel::ePrimary;
	allocInfo.commandBufferCount = 1;
	auto result = m_deviceData.Device.allocateCommandBuffers(allocInfo);
	m_commandBuffer = result.at(0);
}

BufferDeviceLink::~BufferDeviceLink()
{
	//TODO: Destroy the command pool and every resource generated on the device
	ClearBuffers();
}

void BufferDeviceLink::Render(vk::CommandBuffer& _cmd, int _renderPass,
	std::vector<MeshEntry>& _meshEntries, vk::DescriptorSet* _uboSet)
{
	vk::DeviceSize offsets[] = { 0 };

	_cmd.bindVertexBuffers(0, m_drawResources.vertexBuffer, offsets);
	_cmd.bindVertexBuffers(1, m_drawResources.modelMatrixBuffer, offsets);
	_cmd.bindIndexBuffer(m_drawResources.indexBuffer, 0, vk::IndexType::eUint16);

	m_material->RecordCommandBuffer(_cmd, _renderPass, vk::PipelineBindPoint::eGraphics);

	_cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_material->GetLayouts()[0], 0, 1, _uboSet, 0, nullptr);
	_cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_material->GetLayouts()[0], 1, 1, m_descriptorSets.data(), 0, nullptr);

	int currIndex = 0;
	int currInstance = 0;

	for (int i = 0; i < _meshEntries.size(); i++)
	{
		int instanceCount = _meshEntries[i].ModelMatrices.size();
		int indexCount = _meshEntries[i].Data->triangleCount * 3;

		_cmd.drawIndexed(indexCount, instanceCount, currIndex, 0, currInstance);

		currInstance += instanceCount;
		currIndex += indexCount;
	}
}

void BufferDeviceLink::GenerateBuffers(Vertex* _vertexData, uint32_t _vertexCount,
	uint16_t* _indexData, uint32_t _indexCount, 
	glm::mat4x4* _modelData, uint32_t _modelCount)
{
	if (m_resourcesGenerated)
	{
		ClearBuffers();
	}

	//Create the buffers
	BufferCreateInfo vertexBufferInfo
	{
		.buffer = m_drawResources.vertexBuffer,
		.memory = m_drawResources.vertexBufferMemory,
		.usage = vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
		.properties = vk::MemoryPropertyFlagBits::eDeviceLocal,
		.sharingMode = vk::SharingMode::eExclusive,
		.size = sizeof(Vertex) * _vertexCount
	};
	vhf::CreateBufferWithStaging(m_deviceData.Device, m_deviceData.PhysicalDevice, m_commandPool,
		m_deviceData.Queues.graphicsQueue, vertexBufferInfo, _vertexData);

	BufferCreateInfo indexBufferInfo
	{
		.buffer = m_drawResources.indexBuffer,
		.memory = m_drawResources.indexBufferMemory,
		.usage = vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst,
		.properties = vk::MemoryPropertyFlagBits::eDeviceLocal,
		.sharingMode = vk::SharingMode::eExclusive,
		.size = sizeof(uint16_t) * _indexCount
	};
	vhf::CreateBufferWithStaging(m_deviceData.Device, m_deviceData.PhysicalDevice, m_commandPool,
		m_deviceData.Queues.graphicsQueue, indexBufferInfo, _indexData);

	BufferCreateInfo modelMatrixBufferInfo
	{
		.buffer = m_drawResources.modelMatrixBuffer,
		.memory = m_drawResources.modelMatrixBufferMemory,
		.usage = vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst,
		.properties = vk::MemoryPropertyFlagBits::eDeviceLocal,
		.sharingMode = vk::SharingMode::eExclusive,
		.size = sizeof(glm::mat4x4) * _modelCount
	};
	vhf::CreateBufferWithStaging(m_deviceData.Device, m_deviceData.PhysicalDevice, m_commandPool,
		m_deviceData.Queues.graphicsQueue, modelMatrixBufferInfo, _modelData);

	IsDirty = false;
}

void BufferDeviceLink::ClearBuffers()
{
	vk::Device device = m_deviceData.Device;

	//Free the memory
	device.freeMemory(m_drawResources.vertexBufferMemory);
	device.freeMemory(m_drawResources.indexBufferMemory);
	device.freeMemory(m_drawResources.modelMatrixBufferMemory);

	//Destroy the buffers
	device.destroyBuffer(m_drawResources.vertexBuffer);
	device.destroyBuffer(m_drawResources.indexBuffer);
	device.destroyBuffer(m_drawResources.modelMatrixBuffer);
}