#include "Engine/Scene/Scene.h"
#include <glm/vec3.hpp>
#include "Engine/GameObject.h"

#include "Debug/Logger.h"

//TODO: This is only used for testing, remove it later
#include "ResourceManagement/ResourceManager.h"
#include "Engine/Components/BehaviourRegistry.h"

Scene::Scene()
{
	GameObject* testObject = GameObject::CreateAt(glm::vec3(1, 0, 0));

	std::vector<BehaviourRegistryEntry>& entries = BehaviourRegistry::GetRegisteredBehaviours();
	for (auto entry = entries.begin(); entry != entries.end(); entry++)
	{
		ObjectBehaviour* behaviour = entry->CreateFunction(testObject);
	}

	/*std::ofstream fileStream;
	fileStream.open("Scene_Save.txt", std::ios::out | std::ios::trunc);
	if (!fileStream.is_open())
	{
		LOG_ERROR("Couldn't open scene save file for writing");
		return;
	}

	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
	{
		(*it)->SaveToFile(fileStream);
	}

	fileStream.close();*/
}

Scene::~Scene()
{
	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
	{
		delete (*it);
	}
}
