#pragma once

#include <unordered_map>
#include <memory>

#include "MoonlitExport.h"
#include "CustomVulkanStructs.h"

namespace Moonlit::Renderer
{
	class DrawBuffer;
	class BatchGpuData;

	class MOONLIT_API RenderDevice
	{
	public:
		explicit RenderDevice(DeviceData _deviceData);
		~RenderDevice();

		RenderDevice(RenderDevice const&) = delete;
		RenderDevice& operator=(RenderDevice const&) = delete;

		vk::CommandPool GetCommandPool() const { return m_commandPool; }
		DeviceData const& GetDeviceData() const { return m_deviceData; }

		BatchGpuData& GetOrCreateBatchData(DrawBuffer& _drawBuffer);

	private:
		DeviceData m_deviceData;
		vk::CommandPool m_commandPool;
		std::unordered_map<DrawBuffer*, std::unique_ptr<BatchGpuData>> m_batchData;

		void InitCommandPool();
	};
}
