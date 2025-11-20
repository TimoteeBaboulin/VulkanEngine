#include "pch.h"

#include "Behaviours/TestBehaviour.h"

#include <iostream>
#include <string>
#include "Debug/Logger.h"
#include "Engine/GameObject.h"
#include "Behaviours/TransformBehaviour.h"


void TestBehaviour::SubscribeToFunctions()
{
	m_owner->BindToUpdate(std::bind(&TestBehaviour::Update, this));
}

void TestBehaviour::Update()
{
	std::vector<ObjectBehaviour*> transforms;

	if (m_owner->TryGetBehavioursOfType(transforms, typeid(TransformBehaviour)))
	{
		std::string logMessage = "ObjectTest found " + std::to_string(transforms.Size()) + " transforms.";
		Logger::LogInfo(logMessage.c_str());

		for (auto it = transforms.begin(); it != transforms.end(); it++)
		{
			TransformBehaviour* transformPtr = dynamic_cast<TransformBehaviour*>((*it));
			if (transformPtr == nullptr)
			{
				Logger::LogError("Incorrect type received, could not cast to transform type");
			}
		}
	}
}
