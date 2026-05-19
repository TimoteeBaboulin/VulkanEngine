#include "pch.h"

#include <unordered_map>

#include "MeshRendererBehaviour.h"
#include "TransformBehaviour.h"

#include "Engine/Component/GameObject.h"
#include "Engine/MoonlitEngine.h"
#include "Engine/Renderer/Renderer.h"

#include "Engine/ResourceManagement/ResourceManager.h"
#include "Engine/ResourceManagement/ResourceHandle.h"
#include "Debug/Logger.h"

using MeshData = Moonlit::MeshData;

MeshRendererBehaviour::MeshRendererBehaviour(Moonlit::GameObject* _owner)
	: ObjectBehaviour(_owner), m_meshData("MeshData"), m_material("Material")
{
	LookForTransformComponent();

	m_transformChangedSubscriber =
		new Moonlit::Events::ScopedEventSubscriber(m_transformComponent->OnTransformChanged,
		std::bind(&MeshRendererBehaviour::UpdateMeshInstanceModel, this));

	Moonlit::ResourceManagement::ResourceHandle<MeshData> meshHandle;
	if (!Moonlit::ResourceManagement::ResourceManager::TryGetResource<MeshData>("Cube", meshHandle))
	{
		LOG_ERROR("Failed to load default mesh barstool_mesh from ResourceManager.");
		throw std::runtime_error("Failed to load default mesh barstool_mesh from ResourceManager.");
	}
	m_meshData = meshHandle;

	Moonlit::Renderer::MaterialHandle materialHandle;
	if (!Moonlit::ResourceManagement::ResourceManager::TryGetResource<Moonlit::Material>("BaseMaterial", materialHandle))
		LOG_WARNING("MeshRendererBehaviour - No default material 'BaseMaterial' found.");
	m_material = materialHandle;
	OnMaterialChanged();
}

MeshRendererBehaviour::MeshRendererBehaviour(Moonlit::GameObject* _owner, Moonlit::Renderer::MeshHandle _mesh)
	: MeshRendererBehaviour(_owner)
{
	m_meshData = _mesh;
}

MeshRendererBehaviour::~MeshRendererBehaviour()
{
	delete m_transformChangedSubscriber;
	for (ParameterBase* param : m_materialParameters)
		delete param;
}

std::vector<ParameterBase*> MeshRendererBehaviour::GetParameters()
{
	std::vector<ParameterBase*> entries = ObjectBehaviour::GetParameters();
	entries.push_back(&m_meshData);
	entries.push_back(&m_material);
	for (ParameterBase* param : m_materialParameters)
		entries.push_back(param);

	return entries;
}

void MeshRendererBehaviour::ParameterChanged(const ParameterBase* _parameter)
{
	if (_parameter->Name() == "MeshData")
	{
		if (!(*m_meshData).IsValid())
		{
			LOG_WARNING("MeshRendererBehaviour has no valid mesh assigned.");
			return;
		}
		Moonlit::MoonlitEngine::Get().Renderer->UpdateInstanceMesh(m_instanceId, *m_meshData);
	}
	else if (_parameter->Name() == "Material")
	{
		OnMaterialChanged();
	}
	else
	{
		Moonlit::MoonlitEngine::Get().Renderer->UpdateInstanceTextures(m_instanceId, GetTextureHandles());
	}
}

void MeshRendererBehaviour::LoadFromFile(nlohmann::json &_stream)
{
	ObjectBehaviour::LoadFromFile(_stream);
}

void MeshRendererBehaviour::Init()
{
	ObjectBehaviour::Init();

	m_instanceId = Moonlit::MoonlitEngine::Get().Renderer->AddMeshInstance(*m_material, *m_meshData, GetTextureHandles(), m_transformComponent->GetModelMat());
	m_isInitialized = true;
}

void MeshRendererBehaviour::Dispose()
{
	ObjectBehaviour::Dispose();

	Moonlit::MoonlitEngine::Get().Renderer->RemoveMeshInstance(m_instanceId);
}

void MeshRendererBehaviour::LookForTransformComponent()
{
	TransformBehaviour* transform;
	if (!m_owner->TryGetBehaviourOfType<TransformBehaviour>(transform))
	{
		transform = new TransformBehaviour(m_owner);
	}

	m_transformComponent = transform;
}

void MeshRendererBehaviour::UpdateMeshInstanceModel()
{
	Moonlit::MoonlitEngine::Get().Renderer->UpdateInstanceModel(m_instanceId, m_transformComponent->GetModelMat());
}

void MeshRendererBehaviour::OnMaterialChanged()
{
	// Preserve existing texture values by name so a material switch doesn't clear user assignments
	std::unordered_map<std::string, Moonlit::Renderer::TextureHandle> savedTextures;
	for (const ParameterBase* param : m_materialParameters)
	{
		const auto* texParam = dynamic_cast<const Parameter<Moonlit::Renderer::TextureHandle>*>(param);
		if (texParam)
			savedTextures[param->Name()] = texParam->Value();
	}

	for (ParameterBase* param : m_materialParameters)
		delete param;
	m_materialParameters.clear();

	if (!(*m_material).IsValid())
		return;

	std::vector<Moonlit::Renderer::TextureHandle> allTextures;

	for (const Moonlit::ShaderResource& resource : (*m_material)->GetShaderData().GlobalResources)
	{
		if (resource.Type == Moonlit::ResourceType::Texture)
		{
			auto* newParam = new Parameter<Moonlit::Renderer::TextureHandle>(resource.Name);
			auto savedIt = savedTextures.find(resource.Name);
			if (savedIt != savedTextures.end())
			{
				*newParam = savedIt->second;
			}
			else
			{
				if (allTextures.empty())
					allTextures = Moonlit::ResourceManagement::ResourceManager::Get().GetAllResourcesOfType<Moonlit::Image>();
				if (!allTextures.empty())
					*newParam = allTextures[0];
			}
			m_materialParameters.push_back(newParam);
		}
	}

	if (m_isInitialized)
		m_instanceId = Moonlit::MoonlitEngine::Get().Renderer->UpdateInstanceMaterial(
			m_instanceId, *m_material, *m_meshData, GetTextureHandles());
}

std::vector<Moonlit::Renderer::TextureHandle> MeshRendererBehaviour::GetTextureHandles() const
{
	std::vector<Moonlit::Renderer::TextureHandle> textures;
	for (const ParameterBase* param : m_materialParameters)
	{
		const auto* texParam = dynamic_cast<const Parameter<Moonlit::Renderer::TextureHandle>*>(param);
		if (texParam)
			textures.push_back(texParam->Value());
	}
	return textures;
}
