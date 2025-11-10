#include "ResourceManagement/ResourceManager.h"

#include "ResourceManagement/MeshBank.h"
#include "ResourceManagement/TextureBank.h"

MOONLIT_API ResourceManager* ResourceManager::m_instance = nullptr;

ResourceManager::ResourceManager()
{
	m_instance = this;

	MeshBank::Initialize();
	TextureBank::Initialize();

	ResourceManager::RegisterResourceBank<MeshData>(MeshBank::GetInstance());
	ResourceManager::RegisterResourceBank<Image>(TextureBank::GetInstance());
}

ResourceManager* ResourceManager::Get()
{
	if (m_instance == nullptr)
	{
		m_instance = new ResourceManager();
	}
	return m_instance;
}