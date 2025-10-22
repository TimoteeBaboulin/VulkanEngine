#pragma once

#include "Engine/Components/ObjectBehaviour.h"

class PluginBehaviourTest : public ObjectBehaviour
{
public:
	PluginBehaviourTest(GameObject* _owner) : ObjectBehaviour(_owner) {}
	virtual std::vector<ParameterRepositoryEntry> GetParameterEntries() override;
};