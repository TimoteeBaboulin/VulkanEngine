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