#include "ResourceManagement/ResourceManager.h"

#include "ResourceManagement/MeshBank.h"
#include "ResourceManagement/TextureBank.h"
#include "ResourceManagement/FileHelper.h"

#include "Debug/Logger.h"

ResourceManager::ResourceManager()
{
	MeshBank::Initialize();
	TextureBank::Initialize();

	m_resourceBanks[std::type_index(typeid(MeshData))] = MeshBank::GetInstance();
	m_resourceBanks[std::type_index(typeid(Image))] = TextureBank::GetInstance();

	std::vector<std::string> meshFiles = FileHelper::ListFilesInDirectory("Assets/Meshes/");
	for (const std::string& meshFile : meshFiles)
	{
		if (!TryLoadResourceInstance<MeshData>(meshFile))
			LOG_ERROR("ResourceManager::ResourceManager - Failed to load mesh: " + meshFile);
	}
}

ResourceManager* ResourceManager::Instance()
{
	static ResourceManager* Instance = new ResourceManager();
	return Instance;
}