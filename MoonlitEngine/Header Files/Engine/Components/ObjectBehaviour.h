#pragma once

class GameObject;

class ObjectBehaviour
{
public:
	ObjectBehaviour() = delete;
	ObjectBehaviour(GameObject* _owner);

	void SetOwner(GameObject* _owner);

	virtual void SubscribeToFunctions();

protected:
	GameObject* m_owner;
};