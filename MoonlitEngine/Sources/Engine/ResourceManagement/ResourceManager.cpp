#include "ResourceManager.h"

#include "Banks/MeshBank.h"
#include "Banks/TextureBank.h"
#include "Engine/ResourceManagement/Helpers/FileHelper.h"

#include "../../Debug/Logger.h"

#include <filesystem>

Moonlit::ResourceManagement::ResourceManager::ResourceManager()
{
	MeshBank::Initialize();
	TextureBank::Initialize();

	m_resourceBanks[std::type_index(typeid(Moonlit::MeshData))] = (void*)&MeshBank::Get();
	m_resourceBanks[std::type_index(typeid(Moonlit::Image))] = (void*)&TextureBank::Get();

	std::vector<std::string> meshFiles = FileHelper::ListFilesInDirectory("./");
	
	for (const std::string& meshFile : meshFiles)
	{
		LoadFileResource(meshFile);
	}
}

void Moonlit::ResourceManagement::ResourceManager::LoadFileResource(std::string _filepath)
{
	std::filesystem::path filePath;
	filePath = _filepath;
	std::string extension = filePath.extension().string();
	if (extension == ".gltf" || extension == ".fbx" || extension == ".obj")
	{
		if (!TryLoadResourceInternal<MeshData>(_filepath))
			LOG_ERROR("ResourceManager::LoadFileResource - Failed to load mesh: " + _filepath);
		else
			LOG_INFO("ResourceManager::LoadFileResource - Loaded mesh: " + _filepath);
	}
	else if (extension == ".png" || extension == ".jpg" || extension == ".jpeg" || extension == ".bmp")
	{
		if (!TryLoadResourceInternal<Image>(_filepath))
			LOG_ERROR("ResourceManager::LoadFileResource - Failed to load texture: " + _filepath);
		else
			LOG_INFO("ResourceManager::LoadFileResource - Loaded texture: " + _filepath);
	}
}

Moonlit::ResourceManagement::ResourceManager& Moonlit::ResourceManagement::ResourceManager::Get()
{
	static Moonlit::ResourceManagement::ResourceManager instance;
	return instance;
}