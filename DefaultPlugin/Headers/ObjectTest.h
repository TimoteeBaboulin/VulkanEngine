#pragma once

#include "Engine/Components/ObjectBehaviour.h"

class ObjectTest : public ObjectBehaviour
{
public:
	void SubscribeToFunctions() override;

protected:
	void Update();
};