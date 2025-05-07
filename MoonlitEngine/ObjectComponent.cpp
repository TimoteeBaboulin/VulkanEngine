#include "ObjectBehaviour.h"

#include <iostream>

#include "GameObject.h"

ObjectBehaviour::ObjectBehaviour() : m_owner(nullptr)
{
}

void ObjectBehaviour::SetOwner(GameObject* Owner)
{
	m_owner = Owner;
}
