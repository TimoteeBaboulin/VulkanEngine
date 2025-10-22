#include "Engine/Components/ObjectBehaviour.h"

#include <iostream>

#include "Engine/GameObject.h"

ObjectBehaviour::ObjectBehaviour(GameObject* _owner) : m_owner(_owner)
{
	m_owner->AddComponent(this);
}

void ObjectBehaviour::SetOwner(GameObject* _owner)
{
	m_owner = _owner;
	SubscribeToFunctions();
}

void ObjectBehaviour::SubscribeToFunctions()
{
}
