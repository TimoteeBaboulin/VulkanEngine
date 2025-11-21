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
	static uint64_t GetNextId();

	static std::map<uint64_t, GameObject*> m_gameObjects;

	// Default
public:
	GameObject(uint64_t id);
	GameObject(const GameObject& _toCopy);

	virtual ~GameObject();
	virtual GameObject& operator=(const GameObject& _toCopy);
	
	void Update();

	void BindToUpdate(GameEventFunction _func);
	void AddComponent(ObjectBehaviour* _component);

	void SaveToFile(std::ofstream& _stream);
	void LoadFromFile(std::ifstream& _stream);

	GameObject* GetParent() const { return m_parent; }
	void SetParent(GameObject* _parent) { m_parent = _parent; }

	std::vector<GameObject*> GetChildren() const { return m_children; }
	GameObject* GetChildAt(int index) const { return (index < 0 || index >= m_children.size()) ? nullptr : m_children[index]; }
	
	int GetId() const { return m_id; }

	// TODO : Add checks to avoid adding a child multiple times
	
	void AddChild(GameObject* _child) { m_children.push_back(_child); }
	void RemoveChild(GameObject* _child);

	void SetName(const std::string& _name) { m_name = _name; }
	const std::string& GetName() const { return m_name; }

	//Component accessors
	std::vector<ObjectBehaviour*> GetAllBehaviours();

	bool TryGetBehavioursOfType(std::vector<ObjectBehaviour*>& _components, const type_info& _type);
	template <class T>
	bool TryGetBehavioursOfType(std::vector<T*>& _components)
	{
		bool foundComponent = false;

		for (auto it = m_behaviourWidgets.begin(); it != m_behaviourWidgets.end(); it++)
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

	bool TryGetBehaviourOfType(ObjectBehaviour*& _component, const type_info& _type);
	template <class T>
	bool TryGetBehaviourOfType(T*& _component)
	{
		bool foundComponent = false;
		for (auto it = m_behaviourWidgets.begin(); it != m_behaviourWidgets.end(); it++)
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
	uint64_t m_id;
	std::string m_name;

	std::vector<GameEventFunction> m_updates;
	std::vector<ObjectBehaviour*> m_behaviourWidgets;
	std::vector<GameObject*> m_children;
	GameObject* m_parent = nullptr;
};