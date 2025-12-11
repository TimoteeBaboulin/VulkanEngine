#include "ResourceManagement/ResourceManager.h"

#include "ResourceManagement/MeshBank.h"
#include "ResourceManagement/TextureBank.h"
#include "ResourceManagement/FileHelper.h"

#include "Debug/Logger.h"

#include <filesystem>

ResourceManager::ResourceManager()
{
	MeshBank::Initialize();
	TextureBank::Initialize();

	m_resourceBanks[std::type_index(typeid(MeshData))] = MeshBank::GetInstance();
	m_resourceBanks[std::type_index(typeid(Image))] = TextureBank::GetInstance();

	std::vector<std::string> meshFiles = FileHelper::ListFilesInDirectory("./");
	
	for (const std::string& meshFile : meshFiles)
	{
		LoadFileResource(meshFile);
	}
}

void ResourceManager::LoadFileResource(std::string _filepath)
{
	std::filesystem::path filePath;
	filePath = _filepath;
	std::string extension = filePath.extension().string();
	if (extension == ".gltf" || extension == ".fbx" || extension == ".obj")
	{
		if (!TryLoadResourceInstance<MeshData>(_filepath))
			LOG_ERROR("ResourceManager::LoadFileResource - Failed to load mesh: " + _filepath);
		else
			LOG_INFO("ResourceManager::LoadFileResource - Loaded mesh: " + _filepath);
	}
	else if (extension == ".png" || extension == ".jpg" || extension == ".jpeg" || extension == ".bmp")
	{
		if (!TryLoadResourceInstance<Image>(_filepath))
			LOG_ERROR("ResourceManager::LoadFileResource - Failed to load texture: " + _filepath);
		else
			LOG_INFO("ResourceManager::LoadFileResource - Loaded texture: " + _filepath);
	}
	else
	{
		LOG_WARNING("ResourceManager::LoadFileResource - Unsupported file format: " + _filepath);
	}
}

ResourceManager* ResourceManager::Instance()
{
	static ResourceManager* Instance = new ResourceManager();
	return Instance;
}