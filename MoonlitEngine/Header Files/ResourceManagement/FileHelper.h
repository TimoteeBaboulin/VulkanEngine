#pragma once

#include "MoonlitExport.h"

#include <vector>
#include <string>

MOONLIT_API class FileHelper
{
public:
	static std::vector<char> ReadFile(std::string _path);
};
