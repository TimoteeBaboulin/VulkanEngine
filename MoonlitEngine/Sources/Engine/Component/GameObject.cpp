#include "GameObject.h"
#include <iostream>
#include <string>
#include <cstdint>
#include <random>
#include <stdexcept>

#include "ObjectBehaviour.h"
#include "Debug/Logger.h"
#include "BehaviourRegistry.h"

#include "Engine/MoonlitEngine.h"

std::map<uint64_t, Moonlit::GameObject*> Moonlit::GameObject::m_gameObjects = std::map<uint64_t, Moonlit::GameObject*>();
std::vector<Moonlit::GameObjectId> Moonlit::GameObject::m_freeIds = std::vector<Moonlit::GameObjectId>();

Moonlit::GameObject* Moonlit::GameObject::Create()
{
	uint64_t id = GetIndex();
	Scene& scene = MoonlitEngine::GetInstance()->GetScene();
	GameObject* newObject = new GameObject(id, scene);

	return newObject;
}

Moonlit::GameObject* Moonlit::GameObject::CreateAt(glm::vec3 _pos)
{
	GameObject* newObject = Create();

	ObjectBehaviour* transform = BehaviourRegistry::CreateBehaviour("TransformBehaviour", newObject);
	if (!transform)
	{
		LOG_ERROR("GameObject CreateAt: Couldn't create TransformBehaviour for new GameObject");
		delete newObject;
		throw std::invalid_argument("Couldn't find behaviour with name TransformBehaviour");
	}

	transform->SetParameterValue("Position", &_pos);

	return newObject;
}

void Moonlit::GameObject::Destroy(GameObject &_obj) {
	m_freeIds.push_back(_obj.m_id);
	_obj.m_scene.RemoveGameObject(&_obj);
	delete &_obj;
}

Moonlit::GameObjectId Moonlit::GameObject::GetIndex()
{
	GameObjectId id(0);
	if (!m_freeIds.empty()) {
		id = m_freeIds.back();
		id.generation++;
		m_freeIds.pop_back();
	}
	else
	{
		static uint32_t nextIndex = 0;
		id.index = nextIndex++;
		id.generation = 1;
	}

	return id;
}

Moonlit::GameObject::GameObject(GameObjectId _id, Scene& _scene) : m_id(_id), m_scene(_scene)
{
	m_name = "GameObject_" + static_cast<std::string>(m_id);
	m_gameObjects[m_id] = this;
}

Moonlit::GameObject::GameObject(const GameObject& _toCopy) : m_scene(_toCopy.m_scene)
{
	m_id = GetIndex();
	m_gameObjects[m_id] = this;
	m_scene.AddGameObject(this);
}

Moonlit::GameObject::~GameObject()
{
	m_gameObjects.erase(m_id);
}

Moonlit::GameObject& Moonlit::GameObject::operator=(const GameObject& _toCopy)
{
	// TODO: Copy components and children
	return *this;
}

void Moonlit::GameObject::Update()
{
	for (auto it = m_updates.begin(); it != m_updates.end(); it++)
	{
		try
		{
			(*it)();
		}
		catch (const std::exception& err)
		{
			Moonlit::Debug::Logger::LogError("Mistake while updating a component");
			throw (err);
		}
	}
}

void Moonlit::GameObject::BindToUpdate(GameEventFunction _func)
{
	m_updates.push_back(_func);
}

void Moonlit::GameObject::AddComponent(ObjectBehaviour* _component)
{
	m_behaviours.push_back(_component);

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

void Moonlit::GameObject::SaveToFile(std::ofstream& _stream)
{
	const uint32_t magic = 0x474F424A; // 'GOBJ' in ASCII little-endian
	_stream.write(reinterpret_cast<const char*>(&magic), sizeof(magic));

	uint32_t id = m_id;
	_stream.write(reinterpret_cast<const char*>(&id), sizeof(id));

	uint32_t compCount = static_cast<uint32_t>(m_behaviours.size());
	_stream.write(reinterpret_cast<const char*>(&compCount), sizeof(compCount));

	for (auto it = m_behaviours.begin(); it != m_behaviours.end(); it++)
	{
		ObjectBehaviour* component = (*it);
		std::string className = ClassNameFromTypeName(typeid(*component).name());
		WriteStringBinary(_stream, className);
		component->SaveToFile(_stream);
	}
}

void Moonlit::GameObject::LoadFromFile(std::ifstream& _stream)
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
			throw std::invalid_argument("Couldn't create behaviour from scene file, file loading might be corrupted.");
			return;
		}

		m_behaviours.push_back(component);
		component->LoadFromFile(_stream);
	}
}

void Moonlit::GameObject::Destroy() {
	GameObject::Destroy(*this);
}

void Moonlit::GameObject::AddChild(GameObject *_child) {
	auto it = std::find(m_children.begin(), m_children.end(), _child);
	if (it != m_children.end()) {
		LOG_WARNING("Trying to add " + _child->GetName() + " as a child of " + GetName() + ", but it's already a child.");
		return;
	}

	m_children.push_back(_child);
}

void Moonlit::GameObject::RemoveChild(GameObject* _child)
{
	auto it = std::find(m_children.begin(), m_children.end(), _child);
	if (it == m_children.end())
	{
		return;
	}

	m_children.erase(it);
}

std::vector<Moonlit::ObjectBehaviour*> Moonlit::GameObject::GetAllBehaviours()
{
	return m_behaviours;
}

bool Moonlit::GameObject::TryGetBehavioursOfType(std::vector<ObjectBehaviour*>& _components, const type_info& _type)
{
	bool foundComponent = false;

	for (auto it = m_behaviours.begin(); it != m_behaviours.end(); it++)
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

bool Moonlit::GameObject::TryGetBehaviourOfType(ObjectBehaviour*& _component, const type_info& _type)
{
	bool foundComponent = false;
	for (auto it = m_behaviours.begin(); it != m_behaviours.end(); it++)
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

