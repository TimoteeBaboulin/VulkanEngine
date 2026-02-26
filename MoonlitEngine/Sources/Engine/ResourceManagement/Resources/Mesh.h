#pragma once

#include "../../../../Sources/MoonlitExport.h"
#include <vector>
#include <memory>

#include "../../../../Sources/GLMExport.h"

namespace Moonlit
{
	struct MOONLIT_API Vertex
	{
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };

		glm::vec2 uv = { 0.0f, 0.0f };

		glm::vec3 normal = { 0.0f, 0.0f, 0.0f };
		glm::vec3 tangeant = { 0.0f, 0.0f, 0.0f };
		glm::vec3 bitangeant = { 0.0f, 0.0f, 0.0f };
	};
}