#include <set>
#include <string>

#include "vulkan/vulkan.hpp"
#include "Renderer/VulkanDeviceManager.h"
#include "Renderer/VulkanEngine.h"

VulkanDeviceManager::VulkanDeviceManager(vk::SurfaceKHR& _surface) : m_surface(_surface)
{
}

void VulkanDeviceManager::Init(vk::Instance& _instance) 
{
	PickPhysicalDevice(_instance);
	CreateLogicalDevice();
	VulkanEngine::FamilyIndices = m_familyIndices;
}

void VulkanDeviceManager::Cleanup()
{
	VulkanEngine::LogicalDevice.destroy();
}

void VulkanDeviceManager::PickPhysicalDevice(vk::Instance& _instance)
{
	std::vector<vk::PhysicalDevice> physDevices = _instance.enumeratePhysicalDevices();

	if (physDevices.size() == 0)
		throw new std::runtime_error("Failed to find graphics card with Vulkan support");

	for (int i = 0; i < physDevices.size(); i++)
	{
		if (CheckDeviceCompatibility(physDevices[i]))
		{
			VulkanEngine::PhysicalDevice = physDevices[i];
			break;
		}
	}

	if (VulkanEngine::PhysicalDevice == nullptr)
		throw new std::runtime_error("Couldn't find a suitable graphics card");
}

bool VulkanDeviceManager::CheckDeviceCompatibility(vk::PhysicalDevice& _device)
{
	vk::PhysicalDeviceProperties properties = _device.getProperties();
	vk::PhysicalDeviceFeatures features = _device.getFeatures();

	m_familyIndices = GetQueueFamilies(_device);
	bool haveExtensions = CheckDeviceExtensions(_device);

	bool swapChainAdequate = false;
	if (haveExtensions) {
		m_swapChainSupportDetails = QuerySwapChainSupportDetails(_device);
		swapChainAdequate = !m_swapChainSupportDetails.formats.empty() && !m_swapChainSupportDetails.presentModes.empty();
	}

	return m_familyIndices.IsComplete() && haveExtensions && swapChainAdequate;
}

bool VulkanDeviceManager::CheckDeviceExtensions(vk::PhysicalDevice& _device)
{
	std::vector<vk::ExtensionProperties> properties = _device.enumerateDeviceExtensionProperties();
	std::set<std::string> requiredExtensions(m_extensionNames.begin(), m_extensionNames.end());

	for (const auto& extension : properties) {
		// std::cout << extension.extensionName << std::endl;
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

QueueFamilyIndices VulkanDeviceManager::GetQueueFamilies(vk::PhysicalDevice& _device)
{
	QueueFamilyIndices indices;
	auto queueProperties = _device.getQueueFamilyProperties();
	int i = 0;

	for (const auto& property : queueProperties)
	{
		if (property.queueFlags & vk::QueueFlagBits::eGraphics)
			indices.graphicsFamily = i;
		if (_device.getSurfaceSupportKHR(i, m_surface))
			indices.khrPresentFamily = i;

		if (indices.IsComplete())
			break;
		i++;
	}

	return indices;
}

SwapChainSupportDetails VulkanDeviceManager::QuerySwapChainSupportDetails(vk::PhysicalDevice& _device)
{
	SwapChainSupportDetails details;
	unsigned int formatCount = 0;
	unsigned int presentModeCount = 0;

	details.capabilities = _device.getSurfaceCapabilitiesKHR(m_surface);
	details.formats = _device.getSurfaceFormatsKHR(m_surface);
	details.presentModes = _device.getSurfacePresentModesKHR(m_surface);

	return details;
}

void VulkanDeviceManager::CreateLogicalDevice()
{
	float queuePriority = 1.0f;
	std::vector<vk::DeviceQueueCreateInfo> queueInfos;
	std::set<unsigned int> familyIndices{ m_familyIndices.graphicsFamily.value(), m_familyIndices.khrPresentFamily.value() };

	for (auto indice : familyIndices)
	{
		queueInfos.push_back(vk::DeviceQueueCreateInfo{});
		auto it = queueInfos.end();
		it--;
		(*it).sType = vk::StructureType::eDeviceQueueCreateInfo;
		(*it).queueFamilyIndex = m_familyIndices.graphicsFamily.value();
		(*it).queueCount = 1;
		(*it).pQueuePriorities = &queuePriority;
	}

	vk::PhysicalDeviceFeatures targetedFeatures{};
	targetedFeatures.geometryShader = true;

	vk::DeviceCreateInfo createInfo{};
	createInfo.sType = vk::StructureType::eDeviceCreateInfo,
	createInfo.pQueueCreateInfos = queueInfos.data();
	createInfo.queueCreateInfoCount = (uint32_t) queueInfos.size();
	createInfo.pEnabledFeatures = &targetedFeatures;
	createInfo.enabledExtensionCount = (uint32_t) m_extensionNames.size();
	createInfo.ppEnabledExtensionNames = m_extensionNames.data();

	VulkanEngine::LogicalDevice = VulkanEngine::PhysicalDevice.createDevice(createInfo);
}
