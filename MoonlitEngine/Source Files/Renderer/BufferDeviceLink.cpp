#include "Renderer/BufferDeviceLink.h"
#include "Renderer/RendererDeviceManager.h"

BufferDeviceLink::BufferDeviceLink(vk::Device _device, vk::PhysicalDevice _physDevice, RendererDeviceManager& _deviceManager)
{
	m_device = _device;
	m_physDevice = _physDevice;

	vk::CommandPoolCreateInfo poolInfo;
	poolInfo.sType = vk::StructureType::eCommandPoolCreateInfo;
	poolInfo.queueFamilyIndex = _deviceManager.GetQueueFamilyIndices().graphicsFamily.value();
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