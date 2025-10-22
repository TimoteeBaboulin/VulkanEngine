#include "Engine/GameObject.h"
#include <iostream>

#include "Engine/Components/ObjectBehaviour.h"
#include "Debug/Logger.h"
#include "Engine/Components/ObjectTransform.h"

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
	ObjectTransform* transform = new ObjectTransform(newObject, _pos);

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
	_component->SubscribeToFunctions();
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

