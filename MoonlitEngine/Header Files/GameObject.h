#pragma once
#include <stdint.h>
#include <functional>

#include <vector>
#include <map>

#include <typeinfo>

using GameEventFunction = std::function<void(void)>;

class ObjectBehaviour;

class GameObject
{
	friend ObjectBehaviour;
public:
	GameObject(uint32_t id);
	GameObject(const GameObject& _toCopy);

	virtual ~GameObject();
	virtual GameObject& operator=(const GameObject& _toCopy);
	
	void Update();

	void BindToUpdate(GameEventFunction _func);
	void AddComponent(ObjectBehaviour* _component);

	bool TryGetComponentsOfType(std::vector<ObjectBehaviour*>& _components, const type_info& _type);
protected:
	uint32_t m_id;

	std::vector<GameEventFunction> m_updates;
	std::vector<ObjectBehaviour*> m_components;
};