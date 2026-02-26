#include "pch.h"

#include "TestBehaviour.h"

#include <iostream>
#include <string>
#include "Debug/Logger.h"
#include "Engine/Component/GameObject.h"
#include "TransformBehaviour.h"


void TestBehaviour::SubscribeToFunctions()
{
	m_owner->BindToUpdate(std::bind(&TestBehaviour::Update, this));
}

void TestBehaviour::Update()
{
	std::vector<ObjectBehaviour*> transforms;

	if (m_owner->TryGetBehavioursOfType(transforms, typeid(TransformBehaviour)))
	{
		std::string logMessage = "ObjectTest found " + std::to_string(transforms.size()) + " transforms.";
		Moonlit::Debug::Logger::LogInfo(logMessage.c_str());

		for (auto it = transforms.begin(); it != transforms.end(); it++)
		{
			TransformBehaviour* transformPtr = dynamic_cast<TransformBehaviour*>((*it));
			if (transformPtr == nullptr)
			{
				Moonlit::Debug::Logger::LogError("Incorrect type received, could not cast to transform type");
			}
		}
	}
}
