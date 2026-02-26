#pragma once

#include "Engine/Component/ObjectBehaviour.h"

class TestBehaviour : public Moonlit::ObjectBehaviour
{
public:
	void SubscribeToFunctions() override;

protected:
	void Update();
};