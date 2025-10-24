#include "Engine/Components/ObjectBehaviour.h"

#include <iostream>
#include <string>
#include <sstream>
#include <cstdint>

#include "Engine/GameObject.h"
#include "Engine/Components/BehaviourRegistry.h"

static void WriteStringBinary(std::ofstream& s, const std::string& str)
{
    uint32_t len = static_cast<uint32_t>(str.size());
    s.write(reinterpret_cast<const char*>(&len), sizeof(len));
    if (len)
        s.write(str.data(), static_cast<std::streamsize>(len));
}

static bool ReadStringBinary(std::ifstream& s, std::string& out)
{
    uint32_t len = 0;
    if (!s.read(reinterpret_cast<char*>(&len), sizeof(len)))
        return false;
    if (len == 0)
    {
        out.clear();
        return true;
    }
    out.resize(len);
    return static_cast<bool>(s.read(out.data(), static_cast<std::streamsize>(len)));
}

ObjectBehaviour::ObjectBehaviour(GameObject* _owner) : m_owner(_owner)
{
	m_owner->AddComponent(this);
}

void ObjectBehaviour::SetOwner(GameObject* _owner)
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
void ObjectBehaviour::SaveToFile(std::ofstream& _stream)
{
	std::vector<ParameterRepositoryEntry> entries = GetParameterEntries();
	uint32_t paramCount = static_cast<uint32_t>(entries.size());
	_stream.write(reinterpret_cast<const char*>(&paramCount), sizeof(paramCount));

	for (const auto& entry : entries)
	{
		WriteStringBinary(_stream, entry.Name);
		WriteStringBinary(_stream, entry.TypeName);
		uint64_t sz = static_cast<uint64_t>(entry.Size);
		_stream.write(reinterpret_cast<const char*>(&sz), sizeof(sz));
		if (sz > 0 && entry.Data != nullptr)
		{
			_stream.write(reinterpret_cast<const char*>(entry.Data), static_cast<std::streamsize>(sz));
		}
	}
}

void ObjectBehaviour::LoadFromFile(std::ifstream& _stream)
{
	std::vector<ParameterRepositoryEntry> entries = this->GetParameterEntries();

	uint32_t paramCount = 0;
	if (!_stream.read(reinterpret_cast<char*>(&paramCount), sizeof(paramCount)))
	{
		LOG_ERROR("Behaviour Load: Couldn't read data from file");
		return;
	}

	for (uint32_t i = 0; i < paramCount; ++i)
	{
		std::string name;
		std::string typeName;
		if (!ReadStringBinary(_stream, name))
			return;
		if (!ReadStringBinary(_stream, typeName))
			return;

		uint64_t sz = 0;
		if (!_stream.read(reinterpret_cast<char*>(&sz), sizeof(sz)))
			return;

		//Need to find what entry we're referring to
		for (auto it = entries.begin(); it != entries.end(); ++it)
		{
			ParameterRepositoryEntry entry = (*it);
			if (name == entry.Name)
			{
				// If sizes mismatch, only copy the minimum of available and expected to avoid overflow.
				size_t toCopy = static_cast<size_t>(std::min<uint64_t>(entry.Size, sz));
				if (toCopy > 0 && entry.Data != nullptr)
				{
					_stream.read(reinterpret_cast<char*>(entry.Data), static_cast<std::streamsize>(toCopy));
				}
				else if (sz > 0)
				{
					_stream.seekg(static_cast<std::streamoff>(sz), std::ios::cur);
				}
				break;
			}
		}
	}
}

void ObjectBehaviour::SubscribeToFunctions()
{
}

void ObjectBehaviour::SetParameterValue(const std::string& _name, void* _data)
{
	std::vector<ParameterRepositoryEntry> entries = GetParameterEntries();
	for (auto it = entries.begin(); it != entries.end(); it++)
	{
		ParameterRepositoryEntry entry = (*it);
		if (entry.Name == _name)
		{
			std::memcpy(entry.Data, _data, entry.Size);
			return;
		}
	}

}
