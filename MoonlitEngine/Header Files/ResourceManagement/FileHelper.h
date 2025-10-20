#pragma once

#include <vector>
#include <string>
#include "MoonlitExport.h"

MOONLIT_API class FileHelper
{
public:
	static std::vector<char> ReadFile(std::string _path);
};
