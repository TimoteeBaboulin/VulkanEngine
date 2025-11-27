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
	glm::vec3 position = { 0.0f, 0.0f, 0.0f };

	glm::vec2 uv = { 0.0f, 0.0f };

	glm::vec3 normal = { 0.0f, 0.0f, 0.0f };
	glm::vec3 tangeant = { 0.0f, 0.0f, 0.0f };
	glm::vec3 bitangeant = { 0.0f, 0.0f, 0.0f };
};