#pragma once

#include <map>
#include <unordered_map>
#include <memory>

#include "MoonlitExport.h"
#include "CustomVulkanStructs.h"
#include "RendererDeviceManager.h"
#include "BatchGpuData.h"
#include "Material/Material.h"
#include "Material/MaterialInstance.h"

namespace Moonlit::Renderer
{
	class DrawBuffer;
	class BatchGpuData;

	class MOONLIT_API RenderDevice
	{
	public:
		RenderDevice(DeviceData _deviceData, RendererDeviceManager& _deviceManager);
		~RenderDevice();

		RenderDevice(RenderDevice const&) = delete;
		RenderDevice& operator=(RenderDevice const&) = delete;

		vk::CommandPool GetCommandPool() const { return m_commandPool; }
		DeviceData const& GetDeviceData() const { return m_deviceData; }

		SwapChainSupportDetails QuerySwapChainSupportDetails(vk::SurfaceKHR& _surface);

		BatchGpuData& GetOrCreateBatchData(DrawBuffer& _drawBuffer);
		std::unordered_map<DrawBuffer*, std::unique_ptr<BatchGpuData>> const& GetBatchData() const { return m_batchData; }

		MaterialInstance* GetOrCreateMaterialInstance(Material& _material);

	private:
		DeviceData m_deviceData;
		vk::CommandPool m_commandPool;
		RendererDeviceManager* m_deviceManager;
		std::unordered_map<DrawBuffer*, std::unique_ptr<BatchGpuData>> m_batchData;
		std::map<Material*, std::unique_ptr<MaterialInstance>> m_materialInstances;

		void InitCommandPool();
	};
}
