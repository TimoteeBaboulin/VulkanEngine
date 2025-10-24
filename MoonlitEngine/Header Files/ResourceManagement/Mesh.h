#pragma once

#include "MoonlitExport.h"
#include <vector>
#include <memory>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

//TODO: Move out of mesh.h
MOONLIT_API struct Image
{
	unsigned char* pixels;

	uint32_t width;
	uint32_t height;
	int channels;
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
	MeshData* MeshData;
	std::vector<std::shared_ptr<Image>> Textures;
	glm::mat4x4 Model;
};