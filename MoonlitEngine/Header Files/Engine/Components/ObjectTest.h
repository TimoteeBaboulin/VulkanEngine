#pragma once

#include "ObjectBehaviour.h"

class ObjectTest : public ObjectBehaviour
{
public:
	void SubscribeToFunctions() override;

protected:
	void Update();
};