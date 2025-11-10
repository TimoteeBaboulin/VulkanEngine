#include "Engine/GameObject.h"
#include <iostream>
#include <string>
#include <cstdint>
#include <random>

#include "Engine/Components/ObjectBehaviour.h"
#include "Debug/Logger.h"
#include "Engine/Components/BehaviourRegistry.h"

std::map<uint64_t, GameObject*> GameObject::m_gameObjects = std::map<uint64_t, GameObject*>();

GameObject* GameObject::Create()
{
	uint64_t id = GetNextId();
	GameObject* newObject = new GameObject(id);

	return newObject;
}

GameObject* GameObject::CreateAt(glm::vec3 _pos)
{
	GameObject* newObject = Create();

	ObjectBehaviour* transform = BehaviourRegistry::CreateBehaviour("TransformBehaviour", newObject);
	if (!transform)
	{
		Logger::LogError("Failed to create TransformBehaviour for new GameObject");
		delete newObject;
		return nullptr;
	}

	transform->SetParameterValue("Position", &_pos);

	return newObject;
}

uint64_t GameObject::GetNextId()
{
	static std::random_device rd;
	static std::mt19937_64 randEngine(rd());
	static std::uniform_int_distribution<uint64_t> dist(1, UINT64_MAX);

	uint64_t objectId;

	do
	{
		objectId = dist(randEngine);
	} while (m_gameObjects.find(objectId) != m_gameObjects.end());

	return objectId;
}

GameObject::GameObject(uint64_t id) : m_id(id)
{
	m_name = "GameObject_" + std::to_string(m_id);
	m_gameObjects[m_id] = this;
}

GameObject::GameObject(const GameObject& _toCopy)
{
	m_id = GetNextId();
	m_gameObjects[m_id] = this;
}

GameObject::~GameObject()
{
	m_gameObjects.erase(m_id);
}

GameObject& GameObject::operator=(const GameObject& _toCopy)
{
	// TODO: insert return statement here
	return *this;
}

void GameObject::Update()
{
	for (auto it = m_updates.begin(); it != m_updates.end(); it++)
	{
		try
		{
			(*it)();
		}
		catch (const std::exception&)
		{
			Logger::LogError("Mistake while updating a component");
		}
	}
}

void GameObject::BindToUpdate(GameEventFunction _func)
{
	m_updates.push_back(_func);
}

void GameObject::AddComponent(ObjectBehaviour* _component)
{
	m_components.push_back(_component);

	for (auto it = m_components.begin(); it != m_components.end(); it++)
	{
		ObjectBehaviour* component = (*it);
		std::vector<ParameterRepositoryEntry> entries = (*it)->GetParameterEntries();
		for (auto jt = entries.begin(); jt != entries.end(); jt++)
		{
			std::string message = "Registered parameter: ";
			ParameterRepositoryEntry entry = (*jt);
			message += entry.Name;
			message += "\tType: ";
			message += entry.TypeName;
			message += "\tSize: ";
			message += std::to_string(entry.Size);
			message += " bytes";
			Logger::LogInfo(message.c_str());
		}
	}

	_component->SubscribeToFunctions();
}

static void WriteStringBinary(std::ofstream& s, const std::string& str)
{
    uint32_t len = static_cast<uint32_t>(str.size());
    s.write(reinterpret_cast<const char*>(&len), sizeof(len));
    if (len)
        s.write(str.data(), static_cast<std::streamsize>(len));
}

static bool ReadStringBinary(std::ifstream& s, std::string& out)
{
    uint32_t len = 0;
    if (!s.read(reinterpret_cast<char*>(&len), sizeof(len)))
        return false;
    if (len == 0)
    {
        out.clear();
        return true;
    }
    out.resize(len);
    return static_cast<bool>(s.read(out.data(), static_cast<std::streamsize>(len)));
}

void GameObject::SaveToFile(std::ofstream& _stream)
{
	const uint32_t magic = 0x474F424A; // 'GOBJ' in ASCII little-endian
	_stream.write(reinterpret_cast<const char*>(&magic), sizeof(magic));

	uint32_t id = m_id;
	_stream.write(reinterpret_cast<const char*>(&id), sizeof(id));

	uint32_t compCount = static_cast<uint32_t>(m_components.size());
	_stream.write(reinterpret_cast<const char*>(&compCount), sizeof(compCount));

	for (auto it = m_components.begin(); it != m_components.end(); it++)
	{
		ObjectBehaviour* component = (*it);
		std::string className = ClassNameFromTypeName(typeid(*component).name());
		WriteStringBinary(_stream, className);
		component->SaveToFile(_stream);
	}
}

void GameObject::LoadFromFile(std::ifstream& _stream)
{
	// Expected to be called after reading the magic and id
	uint32_t count = 0;
	if (!_stream.read(reinterpret_cast<char*>(&count), sizeof(count)))
		return;

	std::string buffer;
	for (uint32_t i = 0; i < count; ++i)
	{
		if (!ReadStringBinary(_stream, buffer))
			return;

		ObjectBehaviour* component = BehaviourRegistry::CreateBehaviour(buffer, this);
		if (component == nullptr)
		{
			LOG_ERROR("Game Object Load: Couldn't create behaviour from scene file. Can't keep loading file.");
			return;
		}

		m_components.push_back(component);
		component->LoadFromFile(_stream);
	}
}

void GameObject::RemoveChild(GameObject* _child)
{
	auto it = std::find(m_children.begin(), m_children.end(), _child);
	if (it == m_children.end())
	{
		//Open for potential logging or error handling
		return;
	}

	m_children.erase(it);
}

bool GameObject::TryGetComponentsOfType(std::vector<ObjectBehaviour*>& _components, const type_info& _type)
{
	bool foundComponent = false;

	for (auto it = m_components.begin(); it != m_components.end(); it++)
	{
		ObjectBehaviour* component = (*it);
		const type_info& componentType = typeid(*component);
		if (componentType == _type)
		{
			foundComponent = true;
			_components.push_back(component);
		}
	}

	return foundComponent;
}

bool GameObject::TryGetComponentOfType(ObjectBehaviour*& _component, const type_info& _type)
{
	bool foundComponent = false;
	for (auto it = m_components.begin(); it != m_components.end(); it++)
	{
		ObjectBehaviour* component = (*it);
		const type_info& componentType = typeid(*component);
		if (componentType == _type)
		{
			foundComponent = true;
			_component = component;
			break;
		}
	}
	return foundComponent;
}

