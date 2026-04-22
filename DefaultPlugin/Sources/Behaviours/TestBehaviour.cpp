#include "pch.h"

#include "TestBehaviour.h"

#include <iostream>
#include <string>
#include "Debug/Logger.h"
#include "Engine/Component/GameObject.h"
#include "TransformBehaviour.h"


TestBehaviour::TestBehaviour(Moonlit::GameObject *_owner)
	: ObjectBehaviour(_owner)
{

}

void TestBehaviour::SubscribeToFunctions()
{
}

void TestBehaviour::Update(float _dt)
{
	ObjectBehaviour* behaviourPtr = nullptr;

	if (m_owner->TryGetBehaviourOfType(behaviourPtr, typeid(TransformBehaviour)))
	{
		TransformBehaviour& transform = *static_cast<TransformBehaviour*>(behaviourPtr);
		transform.Rotate({0, 0, 360 * _dt});
		transform.Translate({0, 0, 0.5f});
		LOG_INFO("Rotating");
	}
}
