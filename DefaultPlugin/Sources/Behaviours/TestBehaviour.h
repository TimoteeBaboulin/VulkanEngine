#pragma once

#include "Engine/Component/ObjectBehaviour.h"

class TestBehaviour : public Moonlit::ObjectBehaviour
{
public:
	TestBehaviour(Moonlit::GameObject* _owner);

	void SubscribeToFunctions() override;

protected:
	void Update(float _dt) override;
};