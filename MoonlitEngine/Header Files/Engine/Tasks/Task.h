#pragma once

#include <functional>

#include "MoonlitExport.h"

namespace MoonlitEngine
{
	namespace Tasks
	{
		struct MOONLIT_API Task
		{
			std::function<void()> taskFunction;
		};
	}
}