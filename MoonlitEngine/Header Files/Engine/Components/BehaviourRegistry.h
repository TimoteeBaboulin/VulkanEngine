#pragma once

#include <vector>
#include <string>
#include <typeinfo>
#include <cstring>

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

// Helper to remove "class " / "struct " prefix from class names
inline std::string ClassNameFromTypeName(const char* name)
{
	if (!name) return std::string();
	// MSVC usually prefixes class names with "class" or "struct"
	if (std::strncmp(name, "class ", 6) == 0) return std::string(name + 6);
	if (std::strncmp(name, "struct ", 7) == 0) return std::string(name + 7);
	return std::string(name);
}

/// <summary>
/// Static behaviour registry class reprensenting the engine's main reflection tool<para/>
/// All Plugins' behaviours should get registered here to be usable by the engine
/// </summary>
class MOONLIT_API BehaviourRegistry
{
public:
	// Behaviour creation classes with the 3 main uses:
	// Templated, from typeid, or from name

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

		LOG_ERROR("Behaviour of type " + std::string(typeid(T).name()) + " not found in registry");
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
		LOG_ERROR("Behaviour of type " + name + " not found in registry");
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
		LOG_ERROR("Behaviour of type " + std::string(_info.name()) + " not found in registry");
		return nullptr;
	}

	// Functions allowing to add behaviours to the registry
	// This is called while loading a plugin
	// TODO: Add safety checks to avoid adding the same behaviour multiple times

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