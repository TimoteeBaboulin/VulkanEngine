#pragma once

#include <vector>
#include <string>
#include <typeinfo>

#include "MoonlitExport.h"

using BehaviourCreateFunction = void(*)(class GameObject*);

struct MOONLIT_API BehaviourRegistryEntry
{
	std::string Name;
	BehaviourCreateFunction CreateFunction;
};

class MOONLIT_API BehaviourRegistry
{
public:
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
private:
	static std::vector<BehaviourRegistryEntry> Entries;
};