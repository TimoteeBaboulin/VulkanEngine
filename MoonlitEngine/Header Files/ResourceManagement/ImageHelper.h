#pragma once

#include "common.h"
#include "MoonlitExport.h"

#include <string>

MOONLIT_API class ImageHelper
{
public:
	static Image LoadFromFile(std::string _path);
private:
};