#include "Engine/Components/ObjectTest.h"

#include <iostream>
#include <string>
#include "Debug/Logger.h"
#include "Engine/GameObject.h"
#include "Engine/Components/ObjectTransform.h"


void ObjectTest::SubscribeToFunctions()
{
	m_owner->BindToUpdate(std::bind(&ObjectTest::Update, this));
}

void ObjectTest::Update()
{
	std::vector<ObjectBehaviour*> transforms;

	if (m_owner->TryGetComponentsOfType(transforms, typeid(ObjectTransform)))
	{
		std::string logMessage = "ObjectTest found " + std::to_string(transforms.size()) + " transforms.";
		Logger::LogInfo(logMessage.c_str());

		for (auto it = transforms.begin(); it != transforms.end(); it++)
		{
			ObjectTransform* transformPtr = dynamic_cast<ObjectTransform*>((*it));
			if (transformPtr == nullptr)
			{
				Logger::LogError("Incorrect type received, could not cast to transform type");
			}
		}
	}
}
