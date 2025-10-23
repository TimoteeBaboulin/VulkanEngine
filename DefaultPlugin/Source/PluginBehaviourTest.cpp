#include "pch.h"
#include "PluginBehaviourTest.h"

std::vector<ParameterRepositoryEntry> PluginBehaviourTest::GetParameterEntries()
{
	std::vector<ParameterRepositoryEntry> entries;
	int testInt = 42;
	ParameterRepositoryEntry intEntry;
	intEntry.Name = "Test";
	intEntry.Data = nullptr;
	intEntry.Size = 10;
	intEntry.TypeName = typeid(int).name();
	entries.push_back(intEntry);
	return entries;
}