#pragma once

#include "ParameterEditor.h"

#include "Vector3.h"
#include "Float.h"
#include "MeshData.h"
#include "common.h"

#include <glm/vec3.hpp>

using ParamEditorFactory_t = std::function<ParameterEditor* (ParameterRepositoryEntry)>;

struct ParameterWidgetEntry
{
	const char* ParameterTypeName;
	ParamEditorFactory_t FactoryFunction;
};

// Class managing the registry of parameter types and their associated editor factories
// It is a header only class for simplicity
class ParameterTypeRegistry
{
public:
	static ParameterTypeRegistry& Get()
	{
		static ParameterTypeRegistry instance;
		return instance;
	}

	ParameterTypeRegistry()
	{
		// Initialize with default types
		// Note: This is pretty ugly and will be changed later on
		RegisterParameterType(typeid(glm::vec3).name(),
			[](ParameterRepositoryEntry entry) -> ParameterEditor*
			{
				return new Vector3ParameterEditor(entry);
			});
		RegisterParameterType(typeid(std::shared_ptr<MeshData>).name(),
			[](ParameterRepositoryEntry entry) -> ParameterEditor*
			{
				return new Editor::Widgets::MeshDataParameterEditor(entry);
			});
	}

	void RegisterParameterType(const char* _typeName, ParamEditorFactory_t _factory)
	{
		m_registeredTypes.push_back({ _typeName, _factory });
	}

	ParamEditorFactory_t GetFactoryForType(const char* _typeName)
	{
		for (const auto& entry : m_registeredTypes)
		{
			if (strcmp(entry.ParameterTypeName, _typeName) == 0)
			{
				return entry.FactoryFunction;
			}
		}
		return nullptr;
	}

protected:
	std::vector<ParameterWidgetEntry> m_registeredTypes;
};