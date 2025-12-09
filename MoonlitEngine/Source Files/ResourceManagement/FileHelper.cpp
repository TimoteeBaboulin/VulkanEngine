#include "ResourceManagement/FileHelper.h"
#include <fstream>
#include <iostream>
#include <filesystem>
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

std::vector<std::string> FileHelper::ListFilesInDirectory(const std::string& _directoryPath)
{
	std::filesystem::path pathToScan(_directoryPath);

	if (!std::filesystem::exists(pathToScan)) return std::vector<std::string>();

	std::vector<std::string> fileList;

	std::filesystem::recursive_directory_iterator dirIt(pathToScan);
	for (const auto& entry : dirIt)
	{
		if (entry.is_regular_file())
		{
			fileList.push_back(entry.path().string());
		}
	}

	return std::vector<std::string>();
}
