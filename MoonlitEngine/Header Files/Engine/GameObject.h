#pragma once
#include <stdint.h>
#include <functional>

#include <vector>
#include <map>

#include <typeinfo>
#include <glm/vec3.hpp>
#include <fstream>

#include "MoonlitExport.h"

using GameEventFunction = std::function<void(void)>;

class ObjectBehaviour;

/// <summary>
/// Base game object class, not meant to be inherited from<para/>
/// Contains factory functions to create GameObjects
/// </summary>
class MOONLIT_API GameObject
{
	friend ObjectBehaviour;
	// STATIC/FACTORY
public:
	static GameObject* Create();
	static GameObject* CreateAt(glm::vec3 _pos);

private:
	static std::map<uint32_t, GameObject*> m_gameObjects;

	// Default
public:
	GameObject(uint32_t id);
	GameObject(const GameObject& _toCopy);

	virtual ~GameObject();
	virtual GameObject& operator=(const GameObject& _toCopy);
	
	void Update();

	void BindToUpdate(GameEventFunction _func);
	void AddComponent(ObjectBehaviour* _component);

	void SaveToFile(std::ofstream& _stream);
	void LoadFromFile(std::ifstream& _stream);

	//Component accessors
	bool TryGetComponentsOfType(std::vector<ObjectBehaviour*>& _components, const type_info& _type);
	template <class T>
	bool TryGetComponentsOfType(std::vector<T*>& _components)
	{
		bool foundComponent = false;

		for (auto it = m_components.begin(); it != m_components.end(); it++)
		{
			ObjectBehaviour* component = (*it);
			const type_info& componentType = typeid(*component);
			if (componentType == typeid(T))
			{
				foundComponent = true;
				_components.push_back(reinterpret_cast<T*>(component));
			}
		}

		return foundComponent;
	}

	bool TryGetComponentOfType(ObjectBehaviour*& _component, const type_info& _type);
	template <class T>
	bool TryGetComponentOfType(T*& _component)
	{
		bool foundComponent = false;
		for (auto it = m_components.begin(); it != m_components.end(); it++)
		{
			ObjectBehaviour* component = (*it);
			const type_info& componentType = typeid(*component);
			if (componentType == typeid(T))
			{
				foundComponent = true;
				_component = reinterpret_cast<T*>(component);
				break;
			}
		}
		return foundComponent;
	}
protected:
	uint32_t m_id;

	std::vector<GameEventFunction> m_updates;
	std::vector<ObjectBehaviour*> m_components;
};