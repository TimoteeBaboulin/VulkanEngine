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
	BufferDeviceLink(DeviceData _deviceData);
	~BufferDeviceLink();
	DrawBufferResources& const GetBufferResources() { return m_drawResources; };
	vk::Device const GetDevice() { return m_device; }

	void Render();
	void GenerateBuffers(Vertex* _vertexData, uint32_t _vertexCount, );

private:
	DrawBufferResources m_drawResources;
	vk::Device m_device;
	vk::PhysicalDevice m_physDevice;
	vk::CommandPool m_commandPool;
	vk::CommandBuffer m_commandBuffer;

	bool m_resourcesGenerated = false;
};