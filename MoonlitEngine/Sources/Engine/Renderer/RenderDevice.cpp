#include "RenderDevice.h"
#include "BatchGpuData.h"
#include "DrawBuffer.h"

Moonlit::Renderer::RenderDevice::RenderDevice(DeviceData _deviceData, RendererDeviceManager& _deviceManager)
	: m_deviceData(_deviceData), m_deviceManager(&_deviceManager)
{
	InitCommandPool();
}


Moonlit::Renderer::SwapChainSupportDetails Moonlit::Renderer::RenderDevice::QuerySwapChainSupportDetails(vk::SurfaceKHR& _surface)
{
	return m_deviceManager->QuerySwapChainSupportDetails(_surface, m_deviceData.PhysicalDevice);
}

Moonlit::Renderer::RenderDevice::~RenderDevice()
{
	m_deviceData.Device.destroyCommandPool(m_commandPool);
}

Moonlit::Renderer::BatchGpuData& Moonlit::Renderer::RenderDevice::GetOrCreateBatchData(DrawBuffer& _drawBuffer)
{
	auto it = m_batchData.find(&_drawBuffer);
	if (it == m_batchData.end())
	{
		auto batch = std::make_unique<BatchGpuData>(*this, *_drawBuffer.GetMaterial(), &_drawBuffer);
		batch->Initialise();
		auto [newIt, inserted] = m_batchData.emplace(&_drawBuffer, std::move(batch));
		return *newIt->second;
	}
	return *it->second;
}

Moonlit::MaterialInstance* Moonlit::Renderer::RenderDevice::GetOrCreateMaterialInstance(Material& _material)
{
	auto it = m_materialInstances.find(&_material);
	if (it != m_materialInstances.end())
		return it->second.get();

	auto instance = std::make_unique<MaterialInstance>(
		m_deviceData.Device, &_material, m_deviceData.SurfaceFormat.format);
	MaterialInstance* ptr = instance.get();
	m_materialInstances.emplace(&_material, std::move(instance));
	return ptr;
}

void Moonlit::Renderer::RenderDevice::InitCommandPool()
{
	vk::CommandPoolCreateInfo poolInfo;
	poolInfo.sType = vk::StructureType::eCommandPoolCreateInfo;
	poolInfo.queueFamilyIndex = m_deviceData.QueueIndices.graphicsFamily.value();
	poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
	m_commandPool = m_deviceData.Device.createCommandPool(poolInfo);
}
