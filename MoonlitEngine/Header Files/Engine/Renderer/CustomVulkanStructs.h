#ifndef CUSTOM_VULKAN_STRUCTS_H
#define CUSTOM_VULKAN_STRUCTS_H

#include "CustomVulkanImport.h"

#include <optional>
#include <glm/glm.hpp>


struct QueueFamilyIndices
{
	std::optional<unsigned int> graphicsFamily;
	std::optional<unsigned int> khrPresentFamily;

	bool IsComplete()
	{
		return graphicsFamily.has_value() && khrPresentFamily.has_value();
	}
};

struct SwapChainSupportDetails {
	vk::SurfaceCapabilitiesKHR capabilities;
	std::vector<vk::SurfaceFormatKHR> formats;
	std::vector<vk::PresentModeKHR> presentModes;
};

struct RenderInfo
{
	vk::RenderPass renderPass;
	vk::Pipeline pipeline;
	vk::Pipeline depthPipeline;
	vk::PipelineLayout pipelineLayout;
};

struct UniformBufferObject {
	glm::mat4 view;
	glm::mat4 proj;

	glm::vec3 cameraPos;
	float padding = 0.0f;
	glm::vec3 lightPos;
	float padding2 = 0.0f;
	glm::vec3 lightColor;
	float padding3 = 0.0f;
};

struct RenderQueues
{
	vk::Queue graphicsQueue;
	vk::Queue presentQueue;
};

// TODO: Make it lightweight by removing SwapChainSupportDetails and adding a way to query it if needed
struct DeviceData
{
	vk::Device Device;
	vk::PhysicalDevice PhysicalDevice;

	QueueFamilyIndices QueueIndices;
	RenderQueues Queues;
	SwapChainSupportDetails SwapChainSupportDetails;
	float TimeStampPeriods;
};

struct TransitionInfo
{
	vk::AccessFlags srcAccessFlags;
	vk::AccessFlags dstAccessFlags;

	vk::PipelineStageFlags srcStage;
	vk::PipelineStageFlags dstStage;
};

struct BufferCreateInfo
{
	vk::Buffer& buffer;
	vk::DeviceMemory& memory;

	vk::BufferUsageFlags usage;
	vk::MemoryPropertyFlags properties;
	vk::SharingMode sharingMode = vk::SharingMode::eExclusive;

	uint64_t Size;
};

#endif // !CUSTOM_VULKAN_STRUCTS_H
