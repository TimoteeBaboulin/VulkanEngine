#include <set>
#include <string>

#include "vulkan/vulkan.hpp"
#include "Renderer/RendererDeviceManager.h"
#include "Renderer/RenderTarget.h"

RendererDeviceManager::RendererDeviceManager(vk::Instance _instance)
{
	m_vulkanInstance = _instance;
}

RendererDeviceManager::~RendererDeviceManager()
{
	Cleanup();
}

void RendererDeviceManager::Cleanup()
{
	//TODO: Add cleanup code
}

/// <summary>
/// This is used to add a target and automatically pick a fitting physical device and create a new logical device
/// </summary>
DeviceData RendererDeviceManager::AddTarget(RenderTarget* _target)
{
	DeviceData data;
	vk::SurfaceKHR surface = _target->GetSurfaceKHR();
	bool foundMatchingPhysicalDevice = false;

	// Check if a physical device already exist that can handle the surface
	for (auto deviceDataIt = m_devices.begin(); deviceDataIt != m_devices.end(); deviceDataIt++)
	{
		if (CheckDeviceCompatibility((*deviceDataIt).PhysicalDevice, surface , data))
		{
			data = (*deviceDataIt);

			foundMatchingPhysicalDevice = true;
			
			break;
		}
	}

	// Need to pick a new physical device for the surface
	if (!foundMatchingPhysicalDevice)
	{
		data = PickPhysicalDevice(surface);
		m_physicalDevices.push_back(data.PhysicalDevice);
	}
	
	//TODO: Create the queues as well
	CreateLogicalDevice(data);

	return data;
}

void RendererDeviceManager::RemoveTarget(RenderTarget* _target)
{
	DeviceData data = _target->GetDeviceData();

	for (auto it = m_devices.begin(); it != m_devices.end(); it++)
	{
		if ((*it).Device == data.Device)
		{
			m_devices.erase(it);
			break;
		}
	}

	for (auto it = m_renderTargets.begin(); it != m_renderTargets.end(); it++)
	{
		if ((*it) == _target)
		{
			m_renderTargets.erase(it);
			m_targetCount--;
			break;
		}
	}
}


//bool RendererDeviceManager::TryAddRenderTarget(RenderTarget* _target, DeviceData& _deviceData)
//{
//	if (m_targetCount >= 16)
//	{
//		return false;
//	}
//
//	auto targetIterator = std::find(m_renderTargets.begin(), m_renderTargets.end(), nullptr);
//	(*targetIterator) = _target;
//
//	for (int index = 0; index < m_devices.size(); index++)
//	{
//		if (Check)
//	}
//
//	for (int i = 0; i < m_renderTargets.size(); i++)
//	{
//		if (m_renderTargets[i] == nullptr)
//		{
//			m_renderTargets[i] = _target;
//			m_targetCount++;
//
//
//			return true;
//		}
//	}
//
//	return false;
//}

//RenderQueues RendererDeviceManager::GetRenderQueues(RenderTarget* _target) const
//{
//	int index = 0;
//
//	for (int i = 0; i < m_renderTargets.size(); i++)
//	{
//		if (m_renderTargets[i] == _target)
//		{
//			index = i;
//			break;
//		}
//	}
//
//	RenderQueues queues;
//	queues.graphicsQueue = m_device.getQueue(m_familyIndices.graphicsFamily.value(), index);
//	queues.presentQueue = m_device.getQueue(m_familyIndices.khrPresentFamily.value(), index);
//
//	return queues;
//}

/// <summary>
/// Method used to pick a new physical device for use by a new surface
/// </summary>
/// <returns> Returns a DeviceData struct with Swapchain support details, physical device and queue family indices</returns>
DeviceData RendererDeviceManager::PickPhysicalDevice(vk::SurfaceKHR _surface)
{
	//List every physical devices on the target computer
	std::vector<vk::PhysicalDevice> physDevices = m_vulkanInstance.enumeratePhysicalDevices();

	if (physDevices.size() == 0)
		throw new std::runtime_error("Failed to find graphics card with Vulkan support");

	DeviceData data;

	for (int i = 0; i < physDevices.size(); i++)
	{
		//CheckDeviceCompatibility will take care of updating the swapchain support details
		//And the queue family indices field
		if (CheckDeviceCompatibility(physDevices[i], _surface, data))
		{
			data.PhysicalDevice = physDevices[i];
			return data;
		}
	}

	throw new std::runtime_error("Couldn't find a suitable graphics card");
}

bool RendererDeviceManager::CheckDeviceCompatibility(vk::PhysicalDevice& _device, vk::SurfaceKHR& _surface, DeviceData& _deviceData)
{
	vk::PhysicalDeviceProperties properties = _device.getProperties();
	vk::PhysicalDeviceFeatures features = _device.getFeatures();
	vk::SurfaceCapabilitiesKHR surfaceCapabilities = _device.getSurfaceCapabilitiesKHR(_surface);

	QueueFamilyIndices familyIndices = GetQueueFamilies(_device, _surface);
	SwapChainSupportDetails swapChainSupportDetails;
	bool haveExtensions = CheckDeviceExtensions(_device);
	
	bool swapChainAdequate = false;
	if (haveExtensions) {

		swapChainSupportDetails = QuerySwapChainSupportDetails(_surface, _device);
		swapChainAdequate = !swapChainSupportDetails.formats.empty() && !swapChainSupportDetails.presentModes.empty();
	}

	_deviceData.QueueIndices = familyIndices;
	_deviceData.SwapChainSupportDetails = swapChainSupportDetails;

	return familyIndices.IsComplete() && haveExtensions && swapChainAdequate;
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

void RendererDeviceManager::CreateLogicalDevice(DeviceData& _data)
{
	vk::Device device;
	float* queuePriority = new float[16];
	for (int i = 0; i < 16; i++)
	{
		queuePriority[i] = 1.0f;
	}
	std::vector<vk::DeviceQueueCreateInfo> queueInfos;
	std::set<unsigned int> familyIndices{ _data.QueueIndices.graphicsFamily.value(), _data.QueueIndices.khrPresentFamily.value() };

	//Create a single queue of each family (One graphical queue, one KHRPresentQueue for exemple)
	//Since every render target uses its own Device, 
	for (auto index : familyIndices)
	{
		queueInfos.push_back(vk::DeviceQueueCreateInfo{});
		auto it = queueInfos.end();
		it--;
		(*it).sType = vk::StructureType::eDeviceQueueCreateInfo;
		(*it).queueFamilyIndex = index;
		(*it).queueCount = 1;
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

	_data.Device = _data.PhysicalDevice.createDevice(createInfo);
}
