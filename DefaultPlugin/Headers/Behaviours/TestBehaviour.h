#pragma once

#include "Engine/Components/ObjectBehaviour.h"

class TestBehaviour : public ObjectBehaviour
{
public:
	void SubscribeToFunctions() override;

protected:
	void Update();
};