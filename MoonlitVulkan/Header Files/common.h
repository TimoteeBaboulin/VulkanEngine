#ifndef COMMON_H
#define COMMON_H

#include <optional>
#include <iostream>
#include <fstream>
#include <vector>

#include "glm/glm.hpp"
#include "stb_image.h"

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

	float uvX;
	float uvY;
};

struct Mesh
{
	int vertexCount;
	Vertex* vertices;
	int triangleCount;
	int* indices;
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

struct Image
{
	stbi_uc* pixels;

	uint32_t width;
	uint32_t height;
	int channels;
};

static void ImportImage(std::string _path, Image& _image)
{
	int width = _image.width;
	int height = _image.height;
	_image.pixels = stbi_load(_path.c_str(), &width, &height, &_image.channels, STBI_rgb_alpha);

	if (_image.pixels == nullptr)
	{
		throw std::runtime_error("Failed to load image");
	}
}

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