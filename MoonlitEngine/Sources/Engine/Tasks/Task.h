#pragma once

#include <functional>

#include "../../MoonlitExport.h"

namespace Moonlit::Tasks
{
	struct MOONLIT_API Task
	{
		std::function<void()> taskFunction;
	};
}