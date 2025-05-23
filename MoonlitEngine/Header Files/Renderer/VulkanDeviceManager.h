#pragma once

#include "common.h"
#include "vulkan/vulkan.hpp"

class VulkanDeviceManager
{
public:
	VulkanDeviceManager(vk::SurfaceKHR& _surface);

	void Init(vk::Instance& _instance);
	void Cleanup();

	vk::SurfaceCapabilitiesKHR GetCapabilities() const { return m_swapChainSupportDetails.capabilities; }
	std::vector<vk::PresentModeKHR> GetPresentModes() const { return m_swapChainSupportDetails.presentModes; }
	std::vector<vk::SurfaceFormatKHR> GetFormats() const { return m_swapChainSupportDetails.formats; }
	SwapChainSupportDetails GetSwapChainSupportDetails() const { return m_swapChainSupportDetails; }
private:

	vk::SurfaceKHR& m_surface;

	QueueFamilyIndices m_familyIndices;
	SwapChainSupportDetails m_swapChainSupportDetails;

	const std::vector<const char*> m_extensionNames = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_EXT_HOST_QUERY_RESET_EXTENSION_NAME};

	void PickPhysicalDevice(vk::Instance& _instance);
	bool CheckDeviceCompatibility(vk::PhysicalDevice& _device);
	bool CheckDeviceExtensions(vk::PhysicalDevice& _device);
	QueueFamilyIndices GetQueueFamilies(vk::PhysicalDevice& _device);
	SwapChainSupportDetails QuerySwapChainSupportDetails(vk::PhysicalDevice& _device);

	void CreateLogicalDevice();
};