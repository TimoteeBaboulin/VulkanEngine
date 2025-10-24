#include "Engine/Scene/Scene.h"
#include <glm/vec3.hpp>
#include "Engine/GameObject.h"

#include "Debug/Logger.h"

//TODO: This is only used for testing, remove it later
#include "ResourceManagement/ResourceManager.h"
#include "Engine/Components/BehaviourRegistry.h"

#include <cstdint>

Scene::Scene()
{
	GameObject* testObject = GameObject::CreateAt(glm::vec3(1, 0, 0));
	m_gameObjects.push_back(testObject);
}

Scene::~Scene()
{
	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
	{
		delete (*it);
	}
}

void Scene::Save(std::string _filePath)
{
	std::ofstream fileStream;
	fileStream.open(_filePath, std::ios::out | std::ios::trunc | std::ios::binary);
	if (!fileStream.is_open())
	{
		LOG_ERROR("Scene Save: Couldn't open scene save file for writing");
		return;
	}

	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
	{
		(*it)->SaveToFile(fileStream);
	}

	fileStream.close();
}

void Scene::Load(std::string _filePath)
{
	ClearScene();

	std::ifstream fileStream;
	fileStream.open(_filePath, std::ios::in | std::ios::binary);
	if (!fileStream.is_open())
	{
		LOG_ERROR("Scene Load: Couldn't open scene save file for reading");
		return;
	}

	const uint32_t expectedMagic = 0x474F424A; // 'GOBJ'
	while (true)
	{
		uint32_t magic = 0;
		if (!fileStream.read(reinterpret_cast<char*>(&magic), sizeof(magic)))
			break;

		if (magic != expectedMagic)
		{
			LOG_ERROR("Scene load: invalid object label, aborting");
			break;
		}

		// create empty GameObject and let it load the rest
		GameObject* newObject = GameObject::Create();
		m_gameObjects.push_back(newObject);
		newObject->LoadFromFile(fileStream);
	}

	fileStream.close();
}

void Scene::ClearScene()
{
	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
	{
		delete (*it);
	}
	m_gameObjects.clear();
}
