#include "ResourceManagement/TextureBank.h"

#include <filesystem>
#include <stdexcept>

#include "common.h"
#include "Engine/Renderer/VulkanHelperFunctions.h"
#include "ResourceManagement/ImageHelper.h"
#include "Debug/Logger.h"

bool TextureBank::TryLoad(std::string _filepath)
{
    namespace fs = std::filesystem;

    fs::path path(_filepath);
    if (!fs::is_regular_file(path))
    {
		LOG_ERROR(_filepath + " is not a regular file");
        return false;
    }
    std::string name = path.stem().string();

    if (Exist(name))
    {
        Logger::LogWarning("Mesh with name " + name + " already exist in MeshBank.");
        return false;
    }

	m_resources.push_back(ResourcePair<Image>{
		name,
		ImageHelper::LoadFromFile(_filepath),
	});

	m_resources.back().ResourcePtr = std::shared_ptr<Image>(&m_resources.back().Resource);

    return true;
}

