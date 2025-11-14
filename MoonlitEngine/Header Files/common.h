#pragma once

#ifndef COMMON_H
#define COMMON_H

#include "ResourceManagement/Mesh.h"

//TODO: Clean up this file and remove unused stuff
using MeshCountType = uint32_t;

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
	std::shared_ptr<glm::mat4x4> Model;
};

#endif