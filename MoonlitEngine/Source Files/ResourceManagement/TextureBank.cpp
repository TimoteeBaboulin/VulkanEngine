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
        throw new std::runtime_error(_filepath + " is not a regular file");
    std::string name = path.stem().string();
	std::shared_ptr<Image> texturePtr = std::make_shared<Image>();
	

    if (Exist(name))
    {
		Logger::LogWarning(("Mesh with name " + name + " already exist in MeshBank.").c_str());
        return false;
    }

    *texturePtr.get() = ImageHelper::LoadFromFile(_filepath);

	m_resources.push_back(ResourcePair<Image>{
		name,
		texturePtr
	});

    return true;
}

