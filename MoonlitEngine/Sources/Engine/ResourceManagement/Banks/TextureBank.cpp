#include "TextureBank.h"

#include <filesystem>
#include <stdexcept>

#include "../../../common.h"
#include "Engine/Renderer/Helpers/VulkanHelperFunctions.h"
#include "Engine/ResourceManagement/Helpers/ImageHelper.h"
#include "../../../Debug/Logger.h"

bool Moonlit::ResourceManagement::TextureBank::TryLoad(std::string _filepath)
{
    namespace fs = std::filesystem;

    fs::path path(_filepath);
    if (!fs::is_regular_file(path))
    {
		LOG_ERROR(_filepath + " is not a regular file");
        return false;
    }

    std::string name = path.stem().string();

    if (m_resources.contains(name))
    {
        Debug::Logger::LogWarning("Mesh with name " + name + " already exist in MeshBank.");
        return false;
    }

	InsertResource(name, ImageHelper::LoadFromFile(_filepath));

    return true;
}

