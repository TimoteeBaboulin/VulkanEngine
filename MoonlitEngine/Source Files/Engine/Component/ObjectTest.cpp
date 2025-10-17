#include "Engine/Components/ObjectTest.h"

#include "Engine/GameObject.h"
#include "Engine/Components/ObjectTransform.h"

#include <iostream>

void ObjectTest::SubscribeToFunctions()
{
	m_owner->BindToUpdate(std::bind(&ObjectTest::Update, this));
}

void ObjectTest::Update()
{
	std::vector<ObjectBehaviour*> transforms;

	if (m_owner->TryGetComponentsOfType(transforms, typeid(ObjectTransform)))
	{
		std::cout << "Found " << transforms.size() << " transforms:" << std::endl;

		for (auto it = transforms.begin(); it != transforms.end(); it++)
		{
			ObjectTransform* transformPtr = dynamic_cast<ObjectTransform*>((*it));
			if (transformPtr == nullptr)
			{
				std::cout << "Incorrect type received, could not cast to transform type" << std::endl;
			}
			std::cout << transformPtr->GetPosition()[0] << " " << transformPtr->GetPosition()[1] << " " << transformPtr->GetPosition()[2] << std::endl;
		}
	}
}
