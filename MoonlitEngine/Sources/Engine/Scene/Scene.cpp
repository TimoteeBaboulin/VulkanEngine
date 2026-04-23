#include "Scene.h"
#include <glm/vec3.hpp>
#include "Engine/Component/GameObject.h"

#include "Debug/Logger.h"
#include <nlohmann/json.hpp>

//TODO: This is only used for testing, remove it later
#include "Engine/ResourceManagement/ResourceManager.h"
#include "Engine/Component/BehaviourRegistry.h"

#include <cstdint>
#include "SceneManager.h"

namespace Moonlit
{
    Scene::Scene()
	{
	}

    Scene::Scene(std::string _savePath)
    {
	    Load(_savePath);
    }

    Scene::~Scene()
	{
		for (auto it = m_rootGameObjects.begin(); it != m_rootGameObjects.end(); it++)
		{
			delete (*it);
		}
	}

	void Scene::Save(const std::string& _filePath)
	{
    	nlohmann::json json;

		std::ofstream fileStream;
		fileStream.open(_filePath, std::ios::out | std::ios::trunc | std::ios::binary);
		if (!fileStream.is_open())
		{
			LOG_ERROR("Scene Save: Couldn't open scene save file for writing");
			return;
		}

    	json["root_count"] = m_rootGameObjects.size();

    	nlohmann::json rootsJson;

		for (auto it = m_rootGameObjects.begin(); it != m_rootGameObjects.end(); it++)
		{
			(*it)->SaveToFile(rootsJson);
		}

    	json["roots"] = rootsJson;

		fileStream << json.dump(4);

    	if (!m_savePath.compare(""))
    	{
    		m_savePath = _filePath;
    	}

		fileStream.close();
	}

	void Scene::Load(const std::string& _filePath)
	{
		ClearScene();

		std::ifstream fileStream;
		fileStream.open(_filePath, std::ios::in | std::ios::binary);
		if (!fileStream.is_open())
		{
			LOG_ERROR("Scene Load: Couldn't open scene save file for reading");
			return;
		}

    	nlohmann::json json = nlohmann::json::parse(fileStream);

    	int rootCount = json["root_count"];
    	int id;

    	m_rootGameObjects.reserve(rootCount);

    	for (int i = 0; i < rootCount; i++)
    	{
    		nlohmann::json root = json["roots"][i];
    		id = root["id"].get<uint64_t>();
    		m_rootGameObjects.emplace_back(new GameObject(id, *this));
    		m_rootGameObjects[i]->LoadFromFile(root);
    	}

    	m_savePath = _filePath;

		fileStream.close();
	}

	void Scene::ClearScene()
	{
		for (auto it = m_rootGameObjects.begin(); it != m_rootGameObjects.end(); it++)
		{
			delete (*it);
		}

		m_rootGameObjects.clear();
	}

	void Scene::AddGameObject(GameObject* _gameObject, GameObject* _parent)
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

	void Scene::RemoveGameObject(GameObject *_gameObject) {
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

	void Scene::Start()
    {
	    for (auto obj : m_rootGameObjects)
	    {
		    obj->PropagateToEnabledBehaviours([](ObjectBehaviour* _behaviour){
			    _behaviour->OnSpawn();
		    });
	    }
    }

	void Scene::Update(const float& _deltaTime)
    {
	    for (auto obj : m_rootGameObjects)
	    {
		    obj->Update(_deltaTime);
	    }
    }

	void Scene::LateUpdate(const float& _deltaTime)
    {
	    for (auto obj : m_rootGameObjects)
	    {
		    obj->LateUpdate(_deltaTime);
	    }
    }

	void Scene::PreRender()
    {
	    for (auto obj : m_rootGameObjects)
	    {
	    	obj->PreRender();
	    }
    }

	void Scene::PostRender()
    {
	    for (auto obj : m_rootGameObjects)
	    {
		    obj->PostRender();
	    }
    }

	GameObject* Scene::GetGameObjectById(uint64_t _id) const
	{
		auto it = std::find_if(m_rootGameObjects.begin(), m_rootGameObjects.end(), [_id](GameObject* obj)
			{
				return obj->GetId() == _id;
			});

		return (it != m_rootGameObjects.end()) ? *it : nullptr;
	}

	GameObject* Scene::GetGameObjectByName(const std::string& _name) const
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
}