#include "Engine/Components/ObjectBehaviour.h"

#include <iostream>

#include "Engine/GameObject.h"

ObjectBehaviour::ObjectBehaviour(GameObject* _owner) : m_owner(_owner)
{
	m_owner->AddComponent(this);
}

void ObjectBehaviour::SetOwner(GameObject* _owner)
{
	m_owner = _owner;
	SubscribeToFunctions();
}

void ObjectBehaviour::SaveToFile(std::ofstream& _stream)
{
	std::vector<ParameterRepositoryEntry> entries = GetParameterEntries();
	for (auto it = entries.begin(); it != entries.end(); it++)
	{
		ParameterRepositoryEntry entry = (*it);
		_stream << entry.Name << ": ";
		_stream << std::string(entry.Size, ' ');
		_stream << entry.TypeName << " ";
		_stream.write(reinterpret_cast<char*>(entry.Data), entry.Size);
		_stream << std::endl;
	}
}

void ObjectBehaviour::LoadFromFile(std::ifstream& _stream)
{
	std::vector<ParameterRepositoryEntry> entries = this->GetParameterEntries();

	std::string parameterName;
	std::string strBuffer;
	size_t parameterSize;
	for (int index = 0; index < entries.size(); index++)
	{
		_stream >> parameterName;
		for (auto it = entries.begin(); it != entries.end(); it++)
		{
			ParameterRepositoryEntry entry = (*it);
			if (parameterName == entry.Name)
			{
				_stream >> parameterSize;
				entry.Size = parameterSize;

				_stream >> strBuffer;
				entry.TypeName = strBuffer.c_str();

				_stream.read(reinterpret_cast<char*>(entry.Data), entry.Size);
				break;
			}
		}
	}
}

void ObjectBehaviour::SubscribeToFunctions()
{
}
