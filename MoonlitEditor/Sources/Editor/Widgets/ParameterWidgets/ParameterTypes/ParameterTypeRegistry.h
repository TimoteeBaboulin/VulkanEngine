#pragma once

#include "../ParameterEditorBase.h"
#include "Engine/Component/Parameter.h"

#include "Vector3.h"
#include "Float.h"
#include "common.h"

#include <glm/vec3.hpp>

#include "ResourceHandleEditor.h"
#include "Engine/Renderer/Renderer.h"

using ParamEditorFactory_t = std::function<ParameterEditorBase* (ParameterBase*, Moonlit::ObjectBehaviour*, QWidget*)>;

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
		RegisterParameterType(typeid(Parameter<glm::vec3>).name(),
			[](ParameterBase* _param, Moonlit::ObjectBehaviour* _behaviour, QWidget* _parent) -> ParameterEditorBase*
			{
				return new Moonlit::Editor::Vector3ParameterEditor(static_cast<Parameter<glm::vec3>*>(_param), _behaviour, _parent);
			});

		RegisterParameterType(typeid(Parameter<Moonlit::Renderer::MeshHandle>).name(),
			[](ParameterBase* _param, Moonlit::ObjectBehaviour* _behaviour, QWidget* _parent) -> ParameterEditorBase*
			{
				return new Moonlit::Editor::ResourceHandleParameterEditor<Moonlit::Renderer::MeshHandle>
					(static_cast<Parameter<Moonlit::Renderer::MeshHandle>*>(_param), _behaviour, _parent);
			});
		RegisterParameterType(typeid(Parameter<Moonlit::Renderer::TextureHandle>).name(),
			[](ParameterBase* _param, Moonlit::ObjectBehaviour* _behaviour, QWidget* _parent) -> ParameterEditorBase*
			{
				return new Moonlit::Editor::ResourceHandleParameterEditor<Moonlit::Renderer::TextureHandle>
					(static_cast<Parameter<Moonlit::Renderer::TextureHandle>*>(_param), _behaviour, _parent);
			});
		RegisterParameterType(typeid(Parameter<Moonlit::Renderer::MaterialHandle>).name(),
			[](ParameterBase* _param, Moonlit::ObjectBehaviour* _behaviour, QWidget* _parent) -> ParameterEditorBase*
			{
				return new Moonlit::Editor::ResourceHandleParameterEditor<Moonlit::Renderer::MaterialHandle>
					(static_cast<Parameter<Moonlit::Renderer::MaterialHandle>*>(_param), _behaviour, _parent);
			});
	}

	void RegisterParameterType(const char* _typeName, ParamEditorFactory_t _factory)
	{
		for (auto& entry : m_registeredTypes)
		{
			if (strcmp(entry.ParameterTypeName, _typeName) == 0)
			{
				entry.FactoryFunction = _factory;
				return;
			}
		}
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