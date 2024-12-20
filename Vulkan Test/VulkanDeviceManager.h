#pragma once

#include "common.h"
#include "vulkan/vulkan.hpp"

class VulkanDeviceManager
{
public:
	VulkanDeviceManager(vk::SurfaceKHR& _surface);

	void Init(vk::Instance& _instance);
	void Cleanup();

	SwapChainSupportDetails GetSwapChainSupportDetails() const { return m_swapChainSupportDetails; }
private:

	vk::SurfaceKHR& m_surface;

	QueueFamilyIndices m_familyIndices;
	SwapChainSupportDetails m_swapChainSupportDetails;

	const std::vector<const char*> m_extensionNames = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	void PickPhysicalDevice(vk::Instance& _instance);
	bool CheckDeviceCompatibility(vk::PhysicalDevice& _device);
	bool CheckDeviceExtensions(vk::PhysicalDevice& _device);
	QueueFamilyIndices GetQueueFamilies(vk::PhysicalDevice& _device);
	SwapChainSupportDetails QuerySwapChainSupportDetails(vk::PhysicalDevice& _device);

	void CreateLogicalDevice();
};