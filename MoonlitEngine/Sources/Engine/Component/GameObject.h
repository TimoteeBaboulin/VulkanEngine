#pragma once
#include <stdint.h>
#include <functional>

#include <vector>
#include <map>

#include <typeinfo>
#include <glm/vec3.hpp>
#include <fstream>
#include <string>

#include "MoonlitExport.h"
#include "ObjectBehaviour.h"

using GameEventFunction = std::function<void(void)>;

namespace Moonlit
{
	class ObjectBehaviour;

	struct GameObjectId {
		union {
			struct {
				uint32_t index : 32;
				uint32_t generation : 32;
			};
			uint64_t id = 0;
		};

		GameObjectId() : id(0) {}
		GameObjectId(uint64_t _id) : id(_id) {}

		operator uint64_t() const {
			return id;
		}
		operator std::string() const {
			return "Index: " + std::to_string(index) + " Generation: " + std::to_string(generation);
		}
	};

	/// <summary>
	/// Base game object class, not meant to be inherited from<para/>
	/// Contains factory functions to create GameObjects
	/// </summary>
	class MOONLIT_API GameObject
	{
		friend ObjectBehaviour;

	public:
		static GameObject* Create();
		static GameObject* CreateAt(glm::vec3 _pos);

		static void Destroy(GameObject& _obj);
	private:
		static GameObjectId GetIndex();

		static std::map<uint64_t, GameObject*> m_gameObjects;
		static std::vector<GameObjectId> m_freeIds;

		// Default
	public:
		GameObject(GameObjectId _id);
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

		std::vector<GameObject*> GetChildren() { return m_children; }
		GameObject* GetChildAt(int index) const { return (index < 0 || index >= m_children.size()) ? nullptr : m_children[index]; }

		GameObjectId GetId() const { return m_id; }

		void Destroy();

		void AddChild(GameObject* _child);
		void RemoveChild(GameObject* _child);

		void SetName(const std::string& _name) { m_name = _name; }
		const std::string& GetName() const { return m_name; }

		//Component accessors
		std::vector<ObjectBehaviour*> GetAllBehaviours();

		template <IsBehaviour BEHAVIOUR_TYPE>
		void AddComponent();
		template <IsBehaviour BEHAVIOUR_TYPE, typename... Args>
		void AddComponent(Args... _args);

		template <IsBehaviour BEHAVIOUR_TYPE>
		bool HasComponent();

		template <IsBehaviour BEHAVIOUR_TYPE>
		void RemoveComponent();

		bool TryGetBehavioursOfType(std::vector<ObjectBehaviour*>& _components, const type_info& _type);
		template <IsBehaviour T>
		bool TryGetBehavioursOfType(std::vector<T*>& _components);

		bool TryGetBehaviourOfType(ObjectBehaviour*& _component, const type_info& _type);
		template <IsBehaviour T>
		bool TryGetBehaviourOfType(T*& _component)
		{
			bool foundComponent = false;
			for (auto it = m_behaviours.begin(); it != m_behaviours.end(); it++)
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
		GameObjectId m_id;
		std::string m_name;

		std::vector<size_t> m_behavioursLinked;

		std::vector<GameEventFunction> m_updates;
		std::vector<GameObject*> m_children;
		std::vector<ObjectBehaviour*> m_behaviours;
		GameObject* m_parent = nullptr;
	};

	template<IsBehaviour BEHAVIOUR_TYPE>
	void GameObject::AddComponent() {
		BEHAVIOUR_TYPE* component = new BEHAVIOUR_TYPE(this);
		m_behaviours.emplace_back(&component);
	}

	template<IsBehaviour BEHAVIOUR_TYPE, typename ... Args>
	void GameObject::AddComponent(Args... _args) {
		BEHAVIOUR_TYPE* component = new BEHAVIOUR_TYPE(this, std::forward<Args>(_args)...);
		m_behaviours.emplace_back(&component);
	}

	template<IsBehaviour BEHAVIOUR_TYPE>
	bool GameObject::HasComponent() {
		for (auto it = m_behaviours.begin(); it != m_behaviours.end(); it++)
		{
			ObjectBehaviour* component = (*it);
			const type_info& componentType = typeid(*component);
			if (componentType == typeid(BEHAVIOUR_TYPE))
			{
				return true;
			}
		}
		return false;
	}

	template<IsBehaviour BEHAVIOUR_TYPE>
	void GameObject::RemoveComponent() {
		auto it = std::find_if(m_behaviours.begin(), m_behaviours.end(), [](ObjectBehaviour* component) {
			return typeid(*component) == typeid(BEHAVIOUR_TYPE);
		});
		if (it != m_behaviours.end()) {
			m_behaviours.erase(it);
		}
	}

	template<IsBehaviour T>
	bool GameObject::TryGetBehavioursOfType(std::vector<T *> &_components) {
		bool foundComponent = false;

		for (auto it = m_behaviours.begin(); it != m_behaviours.end(); it++)
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
}
