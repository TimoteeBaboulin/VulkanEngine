#pragma once

#include "common.h"

class RenderTarget;

/// <summary>
/// Manager to streamline vulkan device creation/destruction
/// </summary>
class RendererDeviceManager
{
public:
	RendererDeviceManager(vk::Instance _instance);
	~RendererDeviceManager();

	DeviceData AddTarget(RenderTarget* _target);
	void RemoveTarget(RenderTarget* _target);

	SwapChainSupportDetails QuerySwapChainSupportDetails(vk::SurfaceKHR& _surface, vk::PhysicalDevice _device);

private:
	vk::Instance m_vulkanInstance;
	
	size_t m_targetCount = 0;
	std::vector<DeviceData> m_devices;
	std::vector<RenderTarget*> m_renderTargets;

	std::vector<vk::PhysicalDevice> m_physicalDevices;

	const std::vector<const char*> m_extensionNames = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_EXT_HOST_QUERY_RESET_EXTENSION_NAME};

	void Cleanup();

	DeviceData PickPhysicalDevice(vk::SurfaceKHR _surface);
	bool CheckDeviceCompatibility(vk::PhysicalDevice& _device, vk::SurfaceKHR& _surface, DeviceData& _data);
	bool CheckDeviceExtensions(vk::PhysicalDevice& _device);
	QueueFamilyIndices GetQueueFamilies(vk::PhysicalDevice& _device, vk::SurfaceKHR _surfaceKHR);

	void CreateLogicalDevice(DeviceData& _data);
};