#pragma once

#include <typeindex>

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