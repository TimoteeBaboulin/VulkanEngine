#pragma once

#ifndef COMMON_H
#define COMMON_H

#include <optional>
#include <iostream>
#include <fstream>
#include <vector>

#include <glm/glm.hpp>

#define VK_USE_PLATFORM_WIN32_KHR
#undef MemoryBarrier
#include <vulkan/vulkan.hpp>

#include "MoonlitExport.h"
#include "ResourceManagement/Mesh.h"

//TODO: Clean up this file and remove unused stuff
using MeshCountType = uint32_t;

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
};

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

	uint64_t size;
};

#endif