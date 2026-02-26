#pragma once

#include "../../../../Sources/MoonlitExport.h"

#include <memory>

namespace Moonlit
{
	struct MOONLIT_API Image
	{
		unsigned char* pixels;

		uint32_t width;
		uint32_t height;
		int channels;
	};
}