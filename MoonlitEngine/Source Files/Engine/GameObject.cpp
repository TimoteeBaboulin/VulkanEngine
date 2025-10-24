#include "Engine/GameObject.h"
#include <iostream>
#include <string>
#include <cstdint>

#include "Engine/Components/ObjectBehaviour.h"
#include "Debug/Logger.h"
#include "Engine/Components/BehaviourRegistry.h"

std::map<uint32_t, GameObject*> GameObject::m_gameObjects = std::map<uint32_t, GameObject*>();

GameObject* GameObject::Create()
{
	//TODO: implement ID management
	GameObject* newObject = new GameObject(0);
	m_gameObjects[newObject->m_id] = newObject;

	return newObject;
}

GameObject* GameObject::CreateAt(glm::vec3 _pos)
{
	GameObject* newObject = Create();

	ObjectBehaviour* transform = BehaviourRegistry::CreateBehaviour("TransformBehaviour", newObject);
	transform->SetParameterValue("Position", &_pos);

	return newObject;
}

GameObject::GameObject(uint32_t id) : m_id(id), m_updates(), m_components()
{
}

GameObject::GameObject(const GameObject& _toCopy)
{
	m_id = 0;
}

GameObject::~GameObject()
{
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
	// Expect id and component count (magic already consumed by caller)
	uint32_t id = 0;
	if (!_stream.read(reinterpret_cast<char*>(&id), sizeof(id)))
		return;
	m_id = id;

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

