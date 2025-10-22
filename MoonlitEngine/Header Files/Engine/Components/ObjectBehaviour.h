#pragma once

#include "ObjectParameterRepository.h"
#include <vector>
#include <fstream>

#include <typeindex>

#include "MoonlitExport.h"

class GameObject;

class MOONLIT_API ObjectBehaviour
{
public:
	ObjectBehaviour() = delete;
	ObjectBehaviour(GameObject* _owner);

	void SetOwner(GameObject* _owner);
	void SaveToFile(std::ofstream& _stream);
	void LoadFromFile(std::ifstream& _stream);

	virtual void SubscribeToFunctions();

	virtual std::vector<ParameterRepositoryEntry> GetParameterEntries() { return std::vector<ParameterRepositoryEntry>(); };

protected:
	GameObject* m_owner;
};