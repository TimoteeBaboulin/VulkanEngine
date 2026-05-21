#include "RendererDeviceManager.h"
#include <set>
#include <string>

#include "CustomVulkanStructs.h"
#include "RenderTarget.h"

#include "../../Debug/Logger.h"

#include "Engine/Renderer/Helpers/VulkanHelperFunctions.h"

Moonlit::Renderer::RendererDeviceManager::RendererDeviceManager(vk::Instance _instance)
{
	m_vulkanInstance = _instance;
}

Moonlit::Renderer::DeviceData Moonlit::Renderer::RendererDeviceManager::GetOrCreateDeviceForSurface(vk::SurfaceKHR _surface)
{
	if (m_selectedDeviceData.Device)
	{
		SwapChainSupportDetails details = QuerySwapChainSupportDetails(_surface, m_selectedPhysicalDevice);
		if (!details.formats.empty())
			m_selectedDeviceData.SurfaceFormat = HelperClasses::vhf::GetFormat(m_selectedPhysicalDevice, details.formats);
		return m_selectedDeviceData;
	}

	PickPhysicalDevice(_surface);
	InitKHRQueues(_surface);
	CreateLogicalDevice(m_selectedDeviceData);
	GetQueueHandles(m_selectedDeviceData);
	m_selectedLogicalDevice = m_selectedDeviceData.Device;
	return m_selectedDeviceData;
}

Moonlit::Renderer::RendererDeviceManager::~RendererDeviceManager()
{
	Cleanup();
}

void Moonlit::Renderer::RendererDeviceManager::InitKHRQueues(vk::SurfaceKHR _surface)
{
	GetKHRPresentQueue(_surface);

	SwapChainSupportDetails swapChainSupportDetails = QuerySwapChainSupportDetails(_surface, m_selectedPhysicalDevice);
	bool swapChainAdequate = !swapChainSupportDetails.formats.empty() && !swapChainSupportDetails.presentModes.empty();

	if (swapChainAdequate)
	{
		auto surfaceFormat = Moonlit::Renderer::HelperClasses::vhf::GetFormat(m_selectedPhysicalDevice, swapChainSupportDetails.formats);
		m_selectedDeviceData.SurfaceFormat = surfaceFormat;
	}
}

void Moonlit::Renderer::RendererDeviceManager::Cleanup()
{
	if (m_selectedLogicalDevice)
	{
		m_selectedLogicalDevice.waitIdle();
		m_selectedLogicalDevice.destroy();
	}
}

/// <summary>
/// Method used to pick a new physical device for use by the renderer
/// </summary>
/// <returns> Returns a DeviceData struct with Swapchain support details, physical device and queue family indices</returns>
void Moonlit::Renderer::RendererDeviceManager::PickPhysicalDevice(vk::SurfaceKHR _surface)
{
	std::vector<vk::PhysicalDevice> physDevices = m_vulkanInstance.enumeratePhysicalDevices();

	if (physDevices.empty())
		throw std::runtime_error("Failed to find graphics card with Vulkan support");

	for (auto& device : physDevices)
	{
		if (CheckDeviceCompatibility(device, _surface))
		{
			m_selectedPhysicalDevice = device;
			m_selectedDeviceData.PhysicalDevice = device;
			return;
		}
	}

	LOG_ERROR("RendererDeviceManager::PickPhysicalDevice - Couldn't find a suitable graphics card");
	throw std::runtime_error("Couldn't find a suitable graphics card");
}

void Moonlit::Renderer::RendererDeviceManager::GetQueueHandles(DeviceData& _data)
{
	_data.Queues.graphicsQueue = _data.Device.getQueue(_data.QueueIndices.graphicsFamily.value(), 0);
	_data.Queues.presentQueue = _data.Device.getQueue(_data.QueueIndices.khrPresentFamily.value(), 0);
}

bool Moonlit::Renderer::RendererDeviceManager::CheckDeviceCompatibility(vk::PhysicalDevice& _device, vk::SurfaceKHR _surface)
{
	QueueFamilyIndices familyIndices = GetQueueFamilies(_device);

	if (!CheckDeviceExtensions(_device))
		return false;
	if (!familyIndices.HasGraphicsComputeTransferFamilies())
		return false;

	auto queueProperties = _device.getQueueFamilyProperties();
	bool hasPresentSupport = false;
	for (uint32_t i = 0; i < static_cast<uint32_t>(queueProperties.size()); i++)
	{
		if (_device.getSurfaceSupportKHR(i, _surface))
		{
			hasPresentSupport = true;
			break;
		}
	}

	if (!hasPresentSupport)
		return false;

	m_selectedDeviceData.QueueIndices = familyIndices;
	return true;
}

bool Moonlit::Renderer::RendererDeviceManager::CheckDeviceExtensions(vk::PhysicalDevice& _device)
{
	std::vector<vk::ExtensionProperties> properties = _device.enumerateDeviceExtensionProperties();
	std::set<std::string> requiredExtensions(m_extensionNames.begin(), m_extensionNames.end());

	for (const auto& extension : properties) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

Moonlit::Renderer::QueueFamilyIndices Moonlit::Renderer::RendererDeviceManager::GetQueueFamilies(vk::PhysicalDevice& _device)
{
	QueueFamilyIndices indices;
	auto queueProperties = _device.getQueueFamilyProperties();
	int i = 0;

	for (const auto& property : queueProperties)
	{
		if (property.queueFlags & vk::QueueFlagBits::eGraphics && !indices.graphicsFamily.has_value())
		{
			indices.graphicsFamily = i;
		}
		else if (property.queueFlags & vk::QueueFlagBits::eCompute && !indices.computeFamily.has_value())
		{
			indices.computeFamily = i;
		}
		else if (property.queueFlags & vk::QueueFlagBits::eTransfer && !indices.transferFamily.has_value())
		{
			indices.transferFamily = i;
		}

		if (indices.HasGraphicsComputeTransferFamilies())
			break;
		i++;
	}

	return indices;
}

void Moonlit::Renderer::RendererDeviceManager::GetKHRPresentQueue(vk::SurfaceKHR _surface)
{
	auto queueProperties = m_selectedPhysicalDevice.getQueueFamilyProperties();
	int i = 0;

	for (const auto& property : queueProperties)
	{
		if (m_selectedPhysicalDevice.getSurfaceSupportKHR(i, _surface) && m_selectedDeviceData.QueueIndices.FamilyAlreadyUsed(i))
		{
			m_selectedDeviceData.QueueIndices.khrPresentFamily = i;
			return;
		}
	}
}

Moonlit::Renderer::SwapChainSupportDetails Moonlit::Renderer::RendererDeviceManager::QuerySwapChainSupportDetails(vk::SurfaceKHR& _surface, vk::PhysicalDevice _device)
{
	SwapChainSupportDetails details;
	unsigned int formatCount = 0;
	unsigned int presentModeCount = 0;

	// TODO: Add a way to get those capabilities if needed
	details.capabilities = _device.getSurfaceCapabilitiesKHR(_surface);
	details.formats = _device.getSurfaceFormatsKHR(_surface);
	details.presentModes = _device.getSurfacePresentModesKHR(_surface);

	return details;
}

void Moonlit::Renderer::RendererDeviceManager::CreateLogicalDevice(DeviceData& _data)
{
	vk::Device device;
	float* queuePriority = new float[16];
	for (int i = 0; i < 16; i++)
	{
		queuePriority[i] = 1.0f;
	}
	std::vector<vk::DeviceQueueCreateInfo> queueInfos;
	std::vector<unsigned int> familyIndices = _data.QueueIndices.GetUniqueFamilies();

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

	vk::PhysicalDeviceDynamicRenderingFeatures dynamicRenderingFeatures{};
	dynamicRenderingFeatures.sType = vk::StructureType::ePhysicalDeviceDynamicRenderingFeatures;
	dynamicRenderingFeatures.pNext = &indexingFeatures;
	dynamicRenderingFeatures.dynamicRendering = VK_TRUE;

	vk::DeviceCreateInfo createInfo{};
	createInfo.sType = vk::StructureType::eDeviceCreateInfo,
	createInfo.pQueueCreateInfos = queueInfos.data();
	createInfo.queueCreateInfoCount = (uint32_t) queueInfos.size();
	createInfo.pEnabledFeatures = &targetedFeatures;
	createInfo.enabledExtensionCount = (uint32_t) m_extensionNames.size();
	createInfo.ppEnabledExtensionNames = m_extensionNames.data();
	createInfo.pNext = dynamicRenderingFeatures;

	_data.Device = _data.PhysicalDevice.createDevice(createInfo);
	m_selectedLogicalDevice = _data.Device;
}
