#ifndef COMMON_H
#define COMMON_H

#include <optional>
#include <iostream>
#include <fstream>
#include <vector>

#include "glm/glm.hpp"

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
	vk::PipelineLayout pipelineLayout;

	vk::Buffer* vertexBuffer;
	vk::Buffer* indexBuffer;

	uint32_t vertexCount;
	uint32_t triangleCount;
};

struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

struct Vertex
{
	float x;
	float y;
	float z = 0.0f;

	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;
};

struct TransitionInfo
{
	vk::AccessFlags srcAccessFlags;
	vk::AccessFlags dstAccessFlags;

	vk::PipelineStageFlags srcStage;
	vk::PipelineStageFlags dstStage;
};

struct VertexBufferInfo
{
	vk::Buffer& buffer;
	vk::DeviceMemory& memory;

	vk::BufferUsageFlags usage;
	vk::MemoryPropertyFlags properties;

	uint64_t size;
};

static std::vector<char> readFile(const char* _filename)
{
	std::vector<char> buffer;
	std::ifstream stream;
	stream.open(_filename, std::ios::ate | std::ios::binary);

	if (!stream.is_open())
		std::cout << "fuck" << std::endl;

	buffer.resize((size_t)stream.tellg());
	stream.seekg(0);
	stream.read(buffer.data(), buffer.size());

	stream.close();

	return buffer;
}

#endif