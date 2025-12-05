#pragma once

#include "common.h"
#include "CustomVulkanStructs.h"

class RenderTarget;

/// <summary>
/// Manager to streamline vulkan device creation/destruction
/// </summary>
class RendererDeviceManager
{
public:
	RendererDeviceManager(vk::Instance _instance);
	~RendererDeviceManager();

	void InitKHRQueues(vk::SurfaceKHR _surface);

	SwapChainSupportDetails QuerySwapChainSupportDetails(vk::SurfaceKHR& _surface, vk::PhysicalDevice _device);

	DeviceData GetDeviceData() const { return m_selectedDeviceData; }
private:
	vk::Instance m_vulkanInstance;
	
	size_t m_targetCount = 0;

	vk::PhysicalDevice m_selectedPhysicalDevice;
	vk::Device m_selectedLogicalDevice;
	DeviceData m_selectedDeviceData;

	const std::vector<const char*> m_extensionNames = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_EXT_HOST_QUERY_RESET_EXTENSION_NAME, VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME};

	void Cleanup();
	
	bool CheckDeviceCompatibility(vk::PhysicalDevice& _device);
	bool CheckDeviceExtensions(vk::PhysicalDevice& _device);
	QueueFamilyIndices GetQueueFamilies(vk::PhysicalDevice& _device);
	void GetKHRPresentQueue(vk::SurfaceKHR _surface);

	void PickPhysicalDevice();
	void GetQueueHandles(DeviceData& _data);
	void CreateLogicalDevice(DeviceData& _data);
};