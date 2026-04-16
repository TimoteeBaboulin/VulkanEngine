#include "ObjectBehaviour.h"

#include <iostream>
#include <string>
#include <sstream>
#include <cstdint>
#include <typeindex>

#include "GameObject.h"
#include "BehaviourRegistry.h"

Moonlit::ObjectBehaviour::ObjectBehaviour(GameObject* _owner) : m_owner(_owner)
{
	m_owner->AddComponent(this);
}

Moonlit::ObjectBehaviour::~ObjectBehaviour()
{
	LOG_INFO("ObjectBehaviour::~ObjectBehaviour");
}

void Moonlit::ObjectBehaviour::SetOwner(GameObject* _owner)
{
	m_owner = _owner;
	SubscribeToFunctions();
}

// Parameter Binary Layout
// [param_count:uint32] {
//   [name_len:uint32][name bytes]
//   [type_len:uint32][type bytes]
//   [size:uint64]
//   [raw data bytes...]
// }
void Moonlit::ObjectBehaviour::SaveToFile(nlohmann::json& _json)
{
	nlohmann::json behaviourJson;
	behaviourJson["type"] = typeid(*this).name();

	std::vector<ParameterBase*> entries = GetParameters();
	uint32_t paramCount = static_cast<uint32_t>(entries.size());

	nlohmann::json parametersJson;
	behaviourJson["parameter_count"] = paramCount;

	for (const auto& entry : entries)
	{
		nlohmann::json parameterJson;
		entry->Save(parameterJson);
		parametersJson.push_back(parameterJson);
	}
	behaviourJson["parameters"] = parametersJson;
	_json.push_back(behaviourJson);
}

void Moonlit::ObjectBehaviour::LoadFromFile(nlohmann::json& _stream)
{
	std::vector<ParameterBase*> entries = this->GetParameters();

	uint32_t paramCount = _stream["parameter_count"].get<uint32_t>();

	for (uint32_t i = 0; i < paramCount; ++i)
	{
		std::string name = _stream["parameters"][i]["name"].get<std::string>();

		//Need to find what entry we're referring to
		for (auto it = entries.begin(); it != entries.end(); ++it)
		{
			ParameterBase* entry = (*it);
			if (name == entry->Name())
			{
				entry->Load(_stream["parameters"][i]);
				break;
			}
		}
	}
}

void Moonlit::ObjectBehaviour::SubscribeToFunctions()
{
}