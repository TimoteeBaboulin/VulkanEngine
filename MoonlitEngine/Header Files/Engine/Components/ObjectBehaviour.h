#pragma once

#include <vector>
#include <fstream>

#include <typeindex>

#include "MoonlitExport.h"

class GameObject;

/// <summary>
/// Struct representing a member of an object behaviour.
/// This is used for load/read operations
/// </summary>
struct ParameterRepositoryEntry
{
	const char* Name;
	const char* TypeName;
	size_t Size;

	void* Data;
};

class MOONLIT_API ObjectBehaviour
{
public:
	ObjectBehaviour() = delete;
	ObjectBehaviour(GameObject* _owner);

	void SetOwner(GameObject* _owner);
	void SaveToFile(std::ofstream& _stream);
	void LoadFromFile(std::ifstream& _stream);

	virtual void SubscribeToFunctions();

	/// <summary>
	/// Attempts to set a parameter value by name
	/// There is no type or size checking, so error handling must be done by the caller
	/// </summary>
	void SetParameterValue(const std::string& _name, void* _data);
	virtual std::vector<ParameterRepositoryEntry> GetParameterEntries() { return std::vector<ParameterRepositoryEntry>(); };

protected:
	GameObject* m_owner;
};