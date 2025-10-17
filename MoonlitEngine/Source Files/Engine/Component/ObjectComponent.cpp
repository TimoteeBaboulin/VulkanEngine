#include "Engine/Components/ObjectBehaviour.h"

#include <iostream>

#include "Engine/GameObject.h"

ObjectBehaviour::ObjectBehaviour() : m_owner(nullptr)
{
}

void ObjectBehaviour::SetOwner(GameObject* Owner)
{
	m_owner = Owner;
}
