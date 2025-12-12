#pragma once

#include <vector>
#include <string>
#include "MoonlitExport.h"

class MOONLIT_API FileHelper
{
public:
	static std::vector<char> ReadFile(std::string _path);
	static std::vector<std::string> ListFilesInDirectory(const std::string& _directoryPath);
};
