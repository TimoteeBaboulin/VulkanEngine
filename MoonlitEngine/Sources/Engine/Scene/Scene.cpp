#include "Scene.h"
#include <glm/vec3.hpp>
#include "Engine/Component/GameObject.h"

#include "Debug/Logger.h"

//TODO: This is only used for testing, remove it later
#include "Engine/ResourceManagement/ResourceManager.h"
#include "Engine/Component/BehaviourRegistry.h"

#include <cstdint>

Moonlit::Scene::Scene()
{
}

Moonlit::Scene::~Scene()
{
	for (auto it = m_rootGameObjects.begin(); it != m_rootGameObjects.end(); it++)
	{
		delete (*it);
	}
}

void Moonlit::Scene::Save(std::string _filePath)
{
	std::ofstream fileStream;
	fileStream.open(_filePath, std::ios::out | std::ios::trunc | std::ios::binary);
	if (!fileStream.is_open())
	{
		LOG_ERROR("Scene Save: Couldn't open scene save file for writing");
		return;
	}

	for (auto it = m_rootGameObjects.begin(); it != m_rootGameObjects.end(); it++)
	{
		(*it)->SaveToFile(fileStream);
	}

	fileStream.close();
}

void Moonlit::Scene::Load(std::string _filePath)
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

		uint64_t id = 0;
		if (!fileStream.read(reinterpret_cast<char*>(&id), sizeof(id)))
			break;

		// create empty GameObject and let it load the rest
		GameObject* newObject = new GameObject(id, *this);
		m_rootGameObjects.push_back(newObject);
		newObject->LoadFromFile(fileStream);
	}

	fileStream.close();
}

void Moonlit::Scene::ClearScene()
{
	for (auto it = m_rootGameObjects.begin(); it != m_rootGameObjects.end(); it++)
	{
		delete (*it);
	}
	m_rootGameObjects.clear();
}

void Moonlit::Scene::AddGameObject(GameObject* _gameObject, GameObject* _parent)
{
	if (_parent == nullptr)
	{
		m_rootGameObjects.emplace_back(_gameObject);
	}
	else
	{
		_parent->AddChild(_gameObject);
		_gameObject->SetParent(_parent);
	}
}

void Moonlit::Scene::RemoveGameObject(GameObject *_gameObject) {
	auto it = std::find(m_rootGameObjects.begin(), m_rootGameObjects.end(), _gameObject);
	if (it != m_rootGameObjects.end()) {
		m_rootGameObjects.erase(it);
	}
	else
	{
		GameObject* parent = _gameObject->GetParent();
		if (parent)
		{
			parent->RemoveChild(_gameObject);
			_gameObject->SetParent(nullptr);
		}
	}
}

Moonlit::GameObject* Moonlit::Scene::GetGameObjectById(uint64_t _id) const
{
	auto it = std::find_if(m_rootGameObjects.begin(), m_rootGameObjects.end(), [_id](GameObject* obj)
		{
			return obj->GetId() == _id;
		});

	return (it != m_rootGameObjects.end()) ? *it : nullptr;
}

Moonlit::GameObject* Moonlit::Scene::GetGameObjectByName(const std::string& _name) const
{
	for (auto it = m_rootGameObjects.begin(); it != m_rootGameObjects.end(); it++)
	{
		GameObject* obj = *it;
		if (obj->GetName() == _name)
		{
			return obj;
		}
	}

	return nullptr;
}
