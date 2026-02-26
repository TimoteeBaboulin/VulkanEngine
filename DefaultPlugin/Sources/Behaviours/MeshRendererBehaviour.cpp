#include "pch.h"

#include "MeshRendererBehaviour.h"
#include "TransformBehaviour.h"

#include "Engine/Component/GameObject.h"
#include "Engine/MoonlitEngine.h"
#include "Engine/Renderer/Renderer.h"

#include "Engine/ResourceManagement/ResourceManager.h"
#include "Debug/Logger.h"

using Image = Moonlit::Image;
using MeshData = Moonlit::MeshData;

MeshRendererBehaviour::MeshRendererBehaviour(Moonlit::GameObject* _owner)
	: ObjectBehaviour(_owner)
{
	LookForTransformComponent();

	m_transformChangedSubscriber =
		new Moonlit::Events::ScopedEventSubscriber(m_transformComponent->OnTransformChanged,
		std::bind(&MeshRendererBehaviour::UpdateMeshInstanceModel, this));

	Moonlit::ResourceManagement::ResourceManager* m_manager = Moonlit::ResourceManagement::ResourceManager::Instance();
	if (!m_manager->TryGetResource<MeshData>("Cube", m_meshData))
	{
		LOG_ERROR("Failed to load default mesh barstool_mesh from ResourceManager.");
		throw std::runtime_error("Failed to load default mesh barstool_mesh from ResourceManager.");
	}

	std::shared_ptr<Image> firstTexture;
	if (!Moonlit::ResourceManagement::ResourceManager::TryGetResource<Image>("barstool_albedo", firstTexture))
	{
		LOG_ERROR("Failed to load texture barstool_albedo.png from ResourceManager.");
		throw std::runtime_error("Failed to load texture barstool_albedo.png from ResourceManager.");
	}

	std::vector<std::shared_ptr<Image>> textures;
	textures.push_back(firstTexture);

	m_instanceId = Moonlit::MoonlitEngine::GetInstance()->Renderer->AddMeshInstance(m_meshData, textures, m_transformComponent->GetModelMat());
}

MeshRendererBehaviour::MeshRendererBehaviour(Moonlit::GameObject* _owner, std::shared_ptr<MeshData> _mesh) : ObjectBehaviour(_owner)
{
	LookForTransformComponent();

	m_transformChangedSubscriber = new Moonlit::Events::ScopedEventSubscriber(m_transformComponent->OnTransformChanged,
	                                                                          std::bind(&MeshRendererBehaviour::UpdateMeshInstanceModel, this));

	m_meshData = _mesh;

	std::shared_ptr<Image> firstTexture;
	std::vector<std::shared_ptr<Image>> textures;
	if (!Moonlit::ResourceManagement::ResourceManager::TryGetResource<Image>("barstool_albedo", firstTexture))
	{
		LOG_ERROR("Failed to load texture barstool_albedo.png from ResourceManager.");
		throw std::runtime_error("Failed to load texture barstool_albedo.png from ResourceManager.");
	}

	textures.push_back(firstTexture);

	m_instanceId = Moonlit::MoonlitEngine::GetInstance()->Renderer->AddMeshInstance(_mesh, textures, m_transformComponent->GetModelMat());
}

MeshRendererBehaviour::~MeshRendererBehaviour()
{
	delete m_transformChangedSubscriber;
}

std::vector<Moonlit::ParameterRepositoryEntry> MeshRendererBehaviour::GetParameterEntries()
{
	std::vector<Moonlit::ParameterRepositoryEntry> entries = ObjectBehaviour::GetParameterEntries();
	entries.push_back(Moonlit::ParameterRepositoryEntry{
		"Model",
		typeid(m_meshData).name(),
		sizeof(std::shared_ptr<MeshData>),
		&m_meshData
		});

	return entries;
}

void MeshRendererBehaviour::ParameterChanged(const Moonlit::ParameterRepositoryEntry& _parameter)
{
	LOG_INFO("Trying to change parameter " + std::string(_parameter.Name));
	if (strcmp(_parameter.Name, "Model") == 0)
	{
		if (m_meshData == nullptr)
		{
			LOG_WARNING("MeshRendererBehaviour has no valid mesh assigned.");
			return;
		}

		Moonlit::MoonlitEngine::GetInstance()->Renderer->UpdateInstanceMesh(m_instanceId, m_meshData);
	}
	else if (strcmp(_parameter.Name, "Transform") == 0)
	{
		UpdateMeshInstanceModel();
	}
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
	Moonlit::MoonlitEngine::GetInstance()->Renderer->UpdateInstanceModel(m_instanceId, m_transformComponent->GetModelMat());
}
