#pragma once

#ifndef COMMON_H
#define COMMON_H

#include <optional>
#include <iostream>
#include <fstream>
#include <vector>

#include "glm/glm.hpp"

#include "vulkan/vulkan.hpp"

#include "MoonlitExport.h"

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

MOONLIT_API struct Vertex
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;

	float uvX = 0.0f;
	float uvY = 0.0f;
};

MOONLIT_API struct Image
{
	unsigned char* pixels;

	uint32_t width;
	uint32_t height;
	int channels;
};

MOONLIT_API struct MeshData
{
	int vertexCount = 0;
	Vertex* vertices = nullptr;
	int triangleCount = 0;
	uint16_t* indices = nullptr;

	std::vector<Image> textures;
};

struct MeshInstance
{
	MeshData& MeshData;
	std::vector<std::shared_ptr<Image>> Textures;
	glm::mat4x4 Model;
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