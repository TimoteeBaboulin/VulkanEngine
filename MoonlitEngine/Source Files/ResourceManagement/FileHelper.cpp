#include "ResourceManagement/FileHelper.h"
#include <fstream>
#include <iostream>
#include "Debug/Logger.h"

std::vector<char> FileHelper::ReadFile(std::string _path)
{
	std::vector<char> buffer;
	std::ifstream stream;
	stream.open(_path, std::ios::ate | std::ios::binary);

	if (!stream.is_open())
	{
		Logger::LogWarning("Failed to open file: " + _path);
		return buffer;
	}

	buffer.resize((size_t)stream.tellg());
	stream.seekg(0);
	stream.read(buffer.data(), buffer.size());

	stream.close();

	return buffer;
}
