#pragma once

#include <vector>
#include <string>
#include <typeinfo>

#include "MoonlitExport.h"
#include "Engine/Components/ObjectBehaviour.h"
#include "Debug/Logger.h"

class GameObject;
using BehaviourCreateFunction = ObjectBehaviour*(*)(class GameObject*);

struct MOONLIT_API BehaviourRegistryEntry
{
	std::string Name;
	BehaviourCreateFunction CreateFunction;
};
using PluginRegistryFunction = std::vector<BehaviourRegistryEntry>(*)();

class MOONLIT_API BehaviourRegistry
{
public:
	template <typename T>
	static ObjectBehaviour* CreateBehaviour(GameObject* _parent)
	{
		for (auto it = Entries.begin(); it != Entries.end(); it++)
		{
			if (it->Name == typeid(T).name())
			{
				return it->CreateFunction(_parent);
			}
		}

		LOG_ERROR(("Behaviour of type " + std::string(typeid(T).name()) + " not found in registry").c_str());
		return nullptr;
	}
	static ObjectBehaviour* CreateBehaviour(const std::string& name, GameObject* _parent)
	{
		for (auto it = Entries.begin(); it != Entries.end(); it++)
		{
			if (it->Name == name)
			{
				return it->CreateFunction(_parent);
			}
		}
		LOG_ERROR(("Behaviour of type " + name + " not found in registry").c_str());
		return nullptr;
	}
	static ObjectBehaviour* CreateBehaviour(const std::type_info& _info, GameObject* _parent)
	{
		for (auto it = Entries.begin(); it != Entries.end(); it++)
		{
			if (it->Name == _info.name())
			{
				return it->CreateFunction(_parent);
			}
		}
		LOG_ERROR(("Behaviour of type " + std::string(_info.name()) + " not found in registry").c_str());
		return nullptr;
	}

	template <typename T>
	static void RegisterBehaviour(BehaviourCreateFunction createFunction)
	{
		Entries.push_back({ typeid(T).name(), createFunction });
	}
	static void RegisterBehaviour(const std::type_info& _info, BehaviourCreateFunction createFunction)
	{
		Entries.push_back({ _info.name(), createFunction });
	}
	static void RegisterBehaviour(const std::string& name, BehaviourCreateFunction createFunction)
	{
		Entries.push_back({ name, createFunction });
	}

	static std::vector<BehaviourRegistryEntry>& GetRegisteredBehaviours()
	{
		return Entries;
	}
private:
	static std::vector<BehaviourRegistryEntry> Entries;
};