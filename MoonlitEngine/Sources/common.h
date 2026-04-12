#pragma once

#include <nlohmann/json_fwd.hpp>

#include "Engine/ResourceManagement/Resources/Mesh.h"
#include "Engine/ResourceManagement/Resources/Image.h"

namespace Moonlit
{
	//TODO: Clean up this file and remove unused stuff
	using MeshCountType = uint32_t;

	struct MOONLIT_API MeshData
	{
		int vertexCount = 0;
		Vertex* vertices = nullptr;
		int triangleCount = 0;
		uint16_t* indices = nullptr;

		//std::vector<Image> textures;
	};

	struct MeshInstance
	{
		MeshData* MeshData;
		std::vector<std::shared_ptr<Moonlit::Image>> Textures;
		std::shared_ptr<glm::mat4x4> Model;
	};
}