#pragma once

#include "MoonlitExport.h"
#include <vector>
#include <memory>

#include "GLMExport.h"

//TODO: Move out of mesh.h
struct MOONLIT_API Image
{
	unsigned char* pixels;

	uint32_t width;
	uint32_t height;
	int channels;
};

struct MOONLIT_API Vertex
{
	glm::vec3 position = { 0.0f, 0.0f, 0.0f };

	glm::vec2 uv = { 0.0f, 0.0f };

	glm::vec3 normal = { 0.0f, 0.0f, 0.0f };
	glm::vec3 tangeant = { 0.0f, 0.0f, 0.0f };
	glm::vec3 bitangeant = { 0.0f, 0.0f, 0.0f };
};