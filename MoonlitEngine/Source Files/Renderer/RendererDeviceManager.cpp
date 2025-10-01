#include <set>
#include <string>

#include "vulkan/vulkan.hpp"
#include "Renderer/RendererDeviceManager.h"
#include "Renderer/RenderTarget.h"

//RendererDeviceManager::RendererDeviceManager(vk::SurfaceKHR& _surface) : m_surface(_surface)
//{
//	m_renderTargets.resize(16, nullptr);
//}

// Default target is necessary to get the present queue
// TODO: Find a way to not require a default target
RendererDeviceManager::RendererDeviceManager()
{
}

void RendererDeviceManager::Init(vk::Instance& _instance)
{
	PickPhysicalDevice(_instance);

	m_timeStampPeriods = m_physicalDevice.getProperties().limits.timestampPeriod;

	CreateLogicalDevice();
}

void RendererDeviceManager::Cleanup()
{
	m_device.destroy();
}

bool RendererDeviceManager::TryAddRenderTarget(RenderTarget* _target, DeviceData& _deviceData)
{
	if (m_targetCount >= 16)
	{
		return false;
	}

	auto targetIterator = std::find(m_renderTargets.begin(), m_renderTargets.end(), nullptr);
	(*targetIterator) = _target;

	for (int index = 0; index < m_devices.size(); index++)
	{
		if (Check)
	}

	for (int i = 0; i < m_renderTargets.size(); i++)
	{
		if (m_renderTargets[i] == nullptr)
		{
			m_renderTargets[i] = _target;
			m_targetCount++;


			return true;
		}
	}

	return false;
}

RenderQueues RendererDeviceManager::GetRenderQueues(RenderTarget* _target) const
{
	int index = 0;

	for (int i = 0; i < m_renderTargets.size(); i++)
	{
		if (m_renderTargets[i] == _target)
		{
			index = i;
			break;
		}
	}

	RenderQueues queues;
	queues.graphicsQueue = m_device.getQueue(m_familyIndices.graphicsFamily.value(), index);
	queues.presentQueue = m_device.getQueue(m_familyIndices.khrPresentFamily.value(), index);

	return queues;
}

void RendererDeviceManager::PickPhysicalDevice(vk::Instance& _instance)
{
	std::vector<vk::PhysicalDevice> physDevices = _instance.enumeratePhysicalDevices();

	if (physDevices.size() == 0)
		throw new std::runtime_error("Failed to find graphics card with Vulkan support");

	for (int i = 0; i < physDevices.size(); i++)
	{
		if (CheckDeviceCompatibility(physDevices[i]))
		{
			m_physicalDevice = physDevices[i];
			break;
		}
	}

	if (m_physicalDevice == nullptr)
		throw new std::runtime_error("Couldn't find a suitable graphics card");
}

bool RendererDeviceManager::CheckDeviceCompatibility(vk::PhysicalDevice& _device, vk::SurfaceCapabilitiesKHR _surface)
{
	vk::PhysicalDeviceProperties properties = _device.getProperties();
	vk::PhysicalDeviceFeatures features = _device.getFeatures();

	m_familyIndices = GetQueueFamilies(_device, _surface);
	bool haveExtensions = CheckDeviceExtensions(_device);
	

	bool swapChainAdequate = false;
	if (haveExtensions) {

		m_swapChainSupportDetails = QuerySwapChainSupportDetails(_surface, _device);
		swapChainAdequate = !m_swapChainSupportDetails.formats.empty() && !m_swapChainSupportDetails.presentModes.empty();
	}

	return m_familyIndices.IsComplete() && haveExtensions && swapChainAdequate;
}

bool RendererDeviceManager::CheckDeviceExtensions(vk::PhysicalDevice& _device)
{
	std::vector<vk::ExtensionProperties> properties = _device.enumerateDeviceExtensionProperties();
	std::set<std::string> requiredExtensions(m_extensionNames.begin(), m_extensionNames.end());

	for (const auto& extension : properties) {
		// std::cout << extension.extensionName << std::endl;
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

QueueFamilyIndices RendererDeviceManager::GetQueueFamilies(vk::PhysicalDevice& _device, vk::SurfaceKHR _surfaceKHR)
{
	QueueFamilyIndices indices;
	auto queueProperties = _device.getQueueFamilyProperties();
	int i = 0;

	for (const auto& property : queueProperties)
	{
		if (property.queueFlags & vk::QueueFlagBits::eGraphics)
		{
			indices.graphicsFamily = i;
		}
		if (_device.getSurfaceSupportKHR(i, _surfaceKHR))
			indices.khrPresentFamily = i;

		if (indices.IsComplete())
			break;
		i++;
	}

	return indices;
}

SwapChainSupportDetails RendererDeviceManager::QuerySwapChainSupportDetails(vk::SurfaceKHR& _surface, vk::PhysicalDevice _device)
{
	SwapChainSupportDetails details;
	unsigned int formatCount = 0;
	unsigned int presentModeCount = 0;

	details.capabilities = _device.getSurfaceCapabilitiesKHR(_surface);
	details.formats = _device.getSurfaceFormatsKHR(_surface);
	details.presentModes = _device.getSurfacePresentModesKHR(_surface);

	return details;
}

void RendererDeviceManager::CreateLogicalDevice()
{
	float* queuePriority = new float[16];
	for (int i = 0; i < 16; i++)
	{
		queuePriority[i] = 1.0f;
	}
	std::vector<vk::DeviceQueueCreateInfo> queueInfos;
	std::set<unsigned int> familyIndices{ m_familyIndices.graphicsFamily.value(), m_familyIndices.khrPresentFamily.value() };

	for (auto indice : familyIndices)
	{
		queueInfos.push_back(vk::DeviceQueueCreateInfo{});
		auto it = queueInfos.end();
		it--;
		(*it).sType = vk::StructureType::eDeviceQueueCreateInfo;
		(*it).queueFamilyIndex = m_familyIndices.graphicsFamily.value();
		(*it).queueCount = 16;
		(*it).pQueuePriorities = queuePriority;
	}

	vk::PhysicalDeviceFeatures targetedFeatures{};
	targetedFeatures.geometryShader = true;
	
	vk::PhysicalDeviceDescriptorIndexingFeatures indexingFeatures;
	indexingFeatures.sType = vk::StructureType::ePhysicalDeviceDescriptorIndexingFeatures;
	indexingFeatures.descriptorBindingPartiallyBound = true;
	indexingFeatures.shaderSampledImageArrayNonUniformIndexing = true;

	vk::DeviceCreateInfo createInfo{};
	createInfo.sType = vk::StructureType::eDeviceCreateInfo,
	createInfo.pQueueCreateInfos = queueInfos.data();
	createInfo.queueCreateInfoCount = (uint32_t) queueInfos.size();
	createInfo.pEnabledFeatures = &targetedFeatures;
	createInfo.enabledExtensionCount = (uint32_t) m_extensionNames.size();
	createInfo.ppEnabledExtensionNames = m_extensionNames.data();
	createInfo.pNext = indexingFeatures;

	m_device = m_physicalDevice.createDevice(createInfo);
}
