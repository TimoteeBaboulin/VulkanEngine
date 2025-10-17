#include "Engine/GameObject.h"
#include "Engine/Components/ObjectBehaviour.h"

#include <iostream>

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
			std::cout << "Mistake while updating a component" << std::endl;
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
	_component->SetOwner(this);
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

