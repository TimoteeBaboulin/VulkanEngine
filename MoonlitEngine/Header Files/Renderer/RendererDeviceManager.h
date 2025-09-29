#pragma once

#include "common.h"
#include "vulkan/vulkan.hpp"

struct RenderQueues
{
	vk::Queue graphicsQueue;
	vk::Queue presentQueue;
};

class RenderTarget;
//TODO: Find a way to check for present queue support without using a RenderTarget
class RendererDeviceManager
{
public:
	RendererDeviceManager(RenderTarget* _defaultTarget);
	//RendererDeviceManager(vk::SurfaceKHR& _surface);

	void Init(vk::Instance& _instance);
	void Cleanup();

	bool TryAddRenderTarget(RenderTarget* _target);

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
	size_t m_targetCount = 0;
	//vk::SurfaceKHR& m_surface;

	vk::Device m_device;
	vk::PhysicalDevice m_physicalDevice;

	std::vector<RenderTarget*> m_renderTargets;

	float m_timeStampPeriods;

	QueueFamilyIndices m_familyIndices;
	SwapChainSupportDetails m_swapChainSupportDetails;

	const std::vector<const char*> m_extensionNames = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_EXT_HOST_QUERY_RESET_EXTENSION_NAME};

	void PickPhysicalDevice(vk::Instance& _instance);
	bool CheckDeviceCompatibility(vk::PhysicalDevice& _device);
	bool CheckDeviceExtensions(vk::PhysicalDevice& _device);
	QueueFamilyIndices GetQueueFamilies(vk::PhysicalDevice& _device, vk::SurfaceKHR _surfaceKHR);

	void CreateLogicalDevice();
};