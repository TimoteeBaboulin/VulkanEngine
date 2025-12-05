#include "Engine/Renderer/RendererDeviceManager.h"
#include <set>
#include <string>

#include "Engine/Renderer/CustomVulkanStructs.h"
#include "Engine/Renderer/RenderTarget.h"

#include "Debug/Logger.h"

RendererDeviceManager::RendererDeviceManager(vk::Instance _instance)
{
	m_vulkanInstance = _instance;
	PickPhysicalDevice();

	vk::Win32SurfaceCreateInfoKHR surfaceCreateInfo{};
	surfaceCreateInfo.sType = vk::StructureType::eWin32SurfaceCreateInfoKHR;
	surfaceCreateInfo.hinstance = GetModuleHandle(nullptr);
	surfaceCreateInfo.hwnd = GetConsoleWindow();

	vk::SurfaceKHR headlessSurface = m_vulkanInstance.createWin32SurfaceKHR(surfaceCreateInfo);

	InitKHRQueues(headlessSurface);
	CreateLogicalDevice(m_selectedDeviceData);
	GetQueueHandles(m_selectedDeviceData);
	m_vulkanInstance.destroySurfaceKHR(headlessSurface);
}

RendererDeviceManager::~RendererDeviceManager()
{
	Cleanup();
}

void RendererDeviceManager::InitKHRQueues(vk::SurfaceKHR _surface)
{
	vk::SurfaceCapabilitiesKHR surfaceCapabilities = m_selectedPhysicalDevice.getSurfaceCapabilitiesKHR(_surface);
	bool swapChainAdequate = false;
	SwapChainSupportDetails swapChainSupportDetails;

	GetKHRPresentQueue(_surface);

	swapChainSupportDetails = QuerySwapChainSupportDetails(_surface, m_selectedPhysicalDevice);
	swapChainAdequate = !swapChainSupportDetails.formats.empty() && !swapChainSupportDetails.presentModes.empty();

	if (swapChainAdequate)
		m_selectedDeviceData.SwapChainSupportDetails = swapChainSupportDetails;
}

void RendererDeviceManager::Cleanup()
{
	m_selectedLogicalDevice.waitIdle();
	m_selectedLogicalDevice.destroy();
}

/// <summary>
/// Method used to pick a new physical device for use by the renderer
/// </summary>
/// <returns> Returns a DeviceData struct with Swapchain support details, physical device and queue family indices</returns>
void RendererDeviceManager::PickPhysicalDevice()
{
	//List every physical devices on the target computer
	std::vector<vk::PhysicalDevice> physDevices = m_vulkanInstance.enumeratePhysicalDevices();

	if (physDevices.size() == 0)
		throw new std::runtime_error("Failed to find graphics card with Vulkan support");

	for (int i = 0; i < physDevices.size(); i++)
	{
		//CheckDeviceCompatibility will take care of updating the swapchain support details
		//And the queue family indices field
		if (CheckDeviceCompatibility(physDevices[i]))
		{
			m_selectedPhysicalDevice = physDevices[i];
			m_selectedDeviceData.PhysicalDevice = physDevices[i];
			return;
		}
	}

	LOG_ERROR("RendererDeviceManager::PickPhysicalDevice - Couldn't find a suitable graphics card");
	throw new std::runtime_error("Couldn't find a suitable graphics card");
}

void RendererDeviceManager::GetQueueHandles(DeviceData& _data)
{
	_data.Queues.graphicsQueue = _data.Device.getQueue(_data.QueueIndices.graphicsFamily.value(), 0);
	_data.Queues.presentQueue = _data.Device.getQueue(_data.QueueIndices.khrPresentFamily.value(), 0);
}

bool RendererDeviceManager::CheckDeviceCompatibility(vk::PhysicalDevice& _device)
{
	vk::PhysicalDeviceProperties properties = _device.getProperties();
	vk::PhysicalDeviceFeatures features = _device.getFeatures();

	QueueFamilyIndices familyIndices = GetQueueFamilies(_device);
	bool haveExtensions = CheckDeviceExtensions(_device);
	
	if (!haveExtensions)
	{
		return false;
	}

	m_selectedDeviceData.QueueIndices = familyIndices;

	return familyIndices.HasGraphicsComputeTransferFamilies() && haveExtensions;
}

bool RendererDeviceManager::CheckDeviceExtensions(vk::PhysicalDevice& _device)
{
	std::vector<vk::ExtensionProperties> properties = _device.enumerateDeviceExtensionProperties();
	std::set<std::string> requiredExtensions(m_extensionNames.begin(), m_extensionNames.end());

	for (const auto& extension : properties) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

QueueFamilyIndices RendererDeviceManager::GetQueueFamilies(vk::PhysicalDevice& _device)
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

void RendererDeviceManager::GetKHRPresentQueue(vk::SurfaceKHR _surface)
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
}
