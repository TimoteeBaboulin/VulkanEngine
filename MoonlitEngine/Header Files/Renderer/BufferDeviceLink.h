#pragma once

#include <vulkan/vulkan.hpp>

class RendererDeviceManager;

struct DrawBufferResources
{
	vk::Buffer vertexBuffer;
	vk::Buffer indexBuffer;
	vk::Buffer modelMatrixBuffer;

	vk::DeviceMemory vertexBufferMemory;
	vk::DeviceMemory indexBufferMemory;
	vk::DeviceMemory modelMatrixBufferMemory;
};

class BufferDeviceLink
{
public:
	BufferDeviceLink(vk::Device _device, vk::PhysicalDevice _physDevice, RendererDeviceManager& _deviceManager);
	~BufferDeviceLink();
	DrawBufferResources& const GetBufferResources() { return m_drawResources; };
	vk::Device const GetDevice() { return m_device; }
private:
	DrawBufferResources m_drawResources;
	vk::Device m_device;
	vk::PhysicalDevice m_physDevice;
	vk::CommandPool m_commandPool;
	vk::CommandBuffer m_commandBuffer;

	bool m_resourcesGenerated = false;
};