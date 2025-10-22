#include "ResourceManagement/ResourceManager.h"

#include "ResourceManagement/MeshBank.h"
#include "ResourceManagement/TextureBank.h"

ResourceManager* ResourceManager::m_instance = nullptr;

ResourceManager::ResourceManager()
{
	m_instance = this;

	MeshBank::Initialize();
	TextureBank::Initialize();

	ResourceManager::RegisterResourceBank<MeshData>(MeshBank::GetInstance());
	ResourceManager::RegisterResourceBank<Image>(TextureBank::GetInstance());
}
