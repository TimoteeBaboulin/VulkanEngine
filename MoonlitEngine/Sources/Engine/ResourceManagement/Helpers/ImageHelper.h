#pragma once

#include "../../../../Sources/common.h"
#include "../../../../Sources/MoonlitExport.h"
#include "Engine/ResourceManagement/Resources/Image.h"

#include <string>

namespace Moonlit
{
	class MOONLIT_API ImageHelper
	{
	public:
		static Moonlit::Image LoadFromFile(std::string _path);
	private:
	};
}