#include "GameObject.h"
#include "ObjectBehaviour.h"
#include "Debug/Logger.h"
#include "BehaviourRegistry.h"
#include "Engine/MoonlitEngine.h"

#include <iostream>
#include <string>
#include <cstdint>
#include <random>
#include <stdexcept>

std::map<uint64_t, Moonlit::GameObject *> Moonlit::GameObject::m_gameObjects = std::map<uint64_t, GameObject*>();
std::vector<Moonlit::GameObjectId> Moonlit::GameObject::m_freeIds = std::vector<GameObjectId>();

Moonlit::GameObject *Moonlit::GameObject::Create()
{
    uint64_t id = GetIndex();
    Scene &scene = MoonlitEngine::Get().GetScene();
    GameObject *newObject = new GameObject(id, scene);

    return newObject;
}

Moonlit::GameObject *Moonlit::GameObject::CreateAt(glm::vec3 _pos)
{
    GameObject *newObject = Create();
    ObjectBehaviour* transform = newObject->AddComponent("TransformBehaviour");

    transform->SetParameterValue<glm::vec3>("Position", _pos);
    return newObject;
}

void Moonlit::GameObject::Destroy(GameObject &_obj)
{
    m_freeIds.push_back(_obj.m_id);
    _obj.m_scene.RemoveGameObject(&_obj);
    delete &_obj;
}

Moonlit::GameObjectId Moonlit::GameObject::GetIndex()
{
    GameObjectId id(0);
    if (!m_freeIds.empty())
    {
        id = m_freeIds.back();
        id.generation++;
        m_freeIds.pop_back();
    } else
    {
        static uint32_t nextIndex = 0;
        id.index = nextIndex++;
        id.generation = 1;
    }

    return id;
}

Moonlit::GameObject::GameObject(GameObjectId _id, Scene &_scene) : m_id(_id), m_scene(_scene)
{
    m_name = "GameObject_" + static_cast<std::string>(m_id);
    m_gameObjects[m_id.index] = this;
}

Moonlit::GameObject::GameObject(const GameObject &_toCopy) : m_scene(_toCopy.m_scene)
{
    m_id = GetIndex();
    m_gameObjects[m_id.index] = this;
    m_scene.AddGameObject(this);
}

Moonlit::GameObject::~GameObject()
{
    for (auto it = m_behaviours.begin(); it != m_behaviours.end(); it++)
    {
        (*it)->Dispose();
        delete (*it);
    }

    m_gameObjects.erase(m_id);
}

Moonlit::GameObject &Moonlit::GameObject::operator=(const GameObject &_toCopy)
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
        } catch (const std::exception &err)
        {
            Debug::Logger::LogError("Mistake while updating a component");
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

Moonlit::ObjectBehaviour* Moonlit::GameObject::AddComponent(std::string _name)
{
    ObjectBehaviour* behaviour = BehaviourRegistry::CreateBehaviour(this, _name);
    behaviour->Init();
    return behaviour;
}

static void WriteStringBinary(std::ofstream &s, const std::string &str)
{
    uint32_t len = static_cast<uint32_t>(str.size());
    s.write(reinterpret_cast<const char *>(&len), sizeof(len));
    if (len)
        s.write(str.data(), static_cast<std::streamsize>(len));
}

static bool ReadStringBinary(std::ifstream &s, std::string &out)
{
    uint32_t len = 0;
    if (!s.read(reinterpret_cast<char *>(&len), sizeof(len)))
        return false;
    if (len == 0)
    {
        out.clear();
        return true;
    }
    out.resize(len);
    return static_cast<bool>(s.read(out.data(), static_cast<std::streamsize>(len)));
}

void Moonlit::GameObject::SaveToFile(nlohmann::json& _json)
{
    nlohmann::json gameObjectJson;
    gameObjectJson["name"] = m_name;
    gameObjectJson["id"] = static_cast<uint64_t>(m_id);
    gameObjectJson["behaviour_count"] = m_behaviours.size();

    nlohmann::json behavioursJson;

    for (auto it = m_behaviours.begin(); it != m_behaviours.end(); it++)
    {
        ObjectBehaviour *component = (*it);
        component->SaveToFile(behavioursJson);
    }
    gameObjectJson["behaviours"] = behavioursJson;
    _json.push_back(gameObjectJson);
}

void Moonlit::GameObject::LoadFromFile(nlohmann::json& _json)
{
    size_t count = _json["behaviour_count"].get<size_t>();
    m_name = _json["name"];
    m_behaviours.reserve(count);

    std::string typeName;
    for (uint32_t i = 0; i < count; ++i)
    {
        typeName = _json["behaviours"][i]["type"].get<std::string>();
        ObjectBehaviour *component = BehaviourRegistry::CreateBehaviour(this, ClassNameFromTypeName(typeName));
        if (component == nullptr)
        {
            LOG_ERROR("Game Object Load: Couldn't create behaviour from scene file. Can't keep loading file.");
            throw std::invalid_argument("Couldn't create behaviour from scene file, file loading might be corrupted.");
        }

        component->LoadFromFile(_json["behaviours"][i]);
        component->Init();
    }
}

void Moonlit::GameObject::Destroy()
{
    Destroy(*this);
}

void Moonlit::GameObject::AddChild(GameObject *_child)
{
    auto it = std::find(m_children.begin(), m_children.end(), _child);
    if (it != m_children.end())
    {
        LOG_WARNING(
            "Trying to add " + _child->GetName() + " as a child of " + GetName() + ", but it's already a child.");
        return;
    }

    m_children.push_back(_child);
    _child->SetParent(this);
}

void Moonlit::GameObject::RemoveChild(GameObject *_child)
{
    auto it = std::find(m_children.begin(), m_children.end(), _child);
    if (it == m_children.end())
    {
        return;
    }

    m_children.erase(it);
}

std::vector<Moonlit::ObjectBehaviour *> Moonlit::GameObject::GetAllBehaviours()
{
    return m_behaviours;
}

bool Moonlit::GameObject::TryGetBehavioursOfType(std::vector<ObjectBehaviour *> &_components, const type_info &_type)
{
    bool foundComponent = false;

    for (auto it = m_behaviours.begin(); it != m_behaviours.end(); it++)
    {
        ObjectBehaviour *component = (*it);
        const type_info &componentType = typeid(*component);
        if (componentType == _type)
        {
            foundComponent = true;
            _components.push_back(component);
        }
    }

    return foundComponent;
}

bool Moonlit::GameObject::TryGetBehaviourOfType(ObjectBehaviour *&_component, const type_info &_type)
{
    bool foundComponent = false;
    for (auto it = m_behaviours.begin(); it != m_behaviours.end(); it++)
    {
        ObjectBehaviour *component = (*it);
        const type_info &componentType = typeid(*component);
        if (componentType == _type)
        {
            foundComponent = true;
            _component = component;
            break;
        }
    }
    return foundComponent;
}

void Moonlit::GameObject::SetEnabled(bool _enabled)
{
    if (_enabled == m_enabled)
    {
        return;
    }

    m_enabled = _enabled;
    PropagateEnable();
}

void Moonlit::GameObject::PropagateEnable()
{
    for (auto it = m_behaviours.begin(); it != m_behaviours.end(); it++)
    {
        if (!(*it)->Enabled())
        {
            continue;
        }

        if (m_enabled)
        {
            (*it)->OnEnable();
        }
        else
        {
            (*it)->OnDisable();
        }
    }
}
