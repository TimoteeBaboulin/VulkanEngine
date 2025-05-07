#pragma once

class GameObject;

class ObjectBehaviour
{
public:
	ObjectBehaviour();

	void SetOwner(GameObject* Owner);

	virtual void SubscribeToFunctions() = 0;

protected:
	GameObject* m_owner;
};