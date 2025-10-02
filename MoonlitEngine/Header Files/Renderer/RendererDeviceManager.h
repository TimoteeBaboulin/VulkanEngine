#pragma once

#include "common.h"
#include "vulkan/vulkan.hpp"

struct RenderQueues
{
	vk::Queue graphicsQueue;
	vk::Queue presentQueue;
};

struct DeviceData
{
	vk::Device Device;
	vk::PhysicalDevice PhysicalDevice;

	QueueFamilyIndices QueueIndices;
	vk::Queue Queues;
	SwapChainSupportDetails SwapChainSupportDetails;
	float TimeStampPeriods;
};

class RenderTarget;
//TODO: Find a way to check for present queue support without using a RenderTarget
class RendererDeviceManager
{
public:
	RendererDeviceManager(vk::Instance _instance);
	~RendererDeviceManager();

	DeviceData AddTarget(RenderTarget* _target);
	void RemoveTarget(RenderTarget* _target);
	
	DeviceData GetDataFromTarget(RenderTarget* _target);

	SwapChainSupportDetails QuerySwapChainSupportDetails(vk::SurfaceKHR& _surface, vk::PhysicalDevice _device);

#pragma region Getters
	vk::Device GetDevice() const { return m_device; }
	vk::PhysicalDevice GetPhysicalDevice() const { return m_physicalDevice; }
	QueueFamilyIndices GetQueueFamilyIndices() const { return m_familyIndices; }
	float GetTimeStampPeriods() const { return m_timeStampPeriods; }
	RenderQueues GetRenderQueues(RenderTarget* _target) const;
	vk::Queue GetDefaultQueue() const { return m_device.getQueue(m_familyIndices.graphicsFamily.value(), 0); }
#pragma endregion //Getters

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