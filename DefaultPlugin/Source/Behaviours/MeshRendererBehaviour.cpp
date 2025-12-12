#include "pch.h"

#include "Behaviours/MeshRendererBehaviour.h"
#include "Behaviours/TransformBehaviour.h"

#include "Engine/GameObject.h"
#include "Engine/MoonlitEngine.h"
#include "Engine/Renderer/Renderer.h"

#include "ResourceManagement/ResourceManager.h"
#include "Debug/Logger.h"
#include "ResourceManagement/Mesh.h"

MeshRendererBehaviour::MeshRendererBehaviour(GameObject* _owner) 
	: ObjectBehaviour(_owner)
{
	LookForTransformComponent();

	m_transformChangedSubscriber = new ScopedEventSubscriber(m_transformComponent->OnTransformChanged,
		std::bind(&MeshRendererBehaviour::UpdateMeshInstanceModel, this));

	ResourceManager* m_manager = ResourceManager::Instance();
	if (!m_manager->TryGetResource<MeshData>("Cube", m_meshData))
	{
		LOG_ERROR("Failed to load default mesh barstool_mesh from ResourceManager.");
		throw std::runtime_error("Failed to load default mesh barstool_mesh from ResourceManager.");
	}

	std::shared_ptr<Image> firstTexture;
	if (!ResourceManager::TryGetResource<Image>("barstool_albedo", firstTexture))
	{
		LOG_ERROR("Failed to load texture barstool_albedo.png from ResourceManager.");
		throw std::runtime_error("Failed to load texture barstool_albedo.png from ResourceManager.");
	}

	std::vector<std::shared_ptr<Image>> textures;
	textures.push_back(firstTexture);

	m_instanceId = MoonlitEngine::GetInstance()->Renderer->AddMeshInstance(m_meshData, textures, m_transformComponent->GetModelMat());
}

MeshRendererBehaviour::MeshRendererBehaviour(GameObject* _owner, std::shared_ptr<MeshData> _mesh) : ObjectBehaviour(_owner)
{
	LookForTransformComponent();

	m_transformChangedSubscriber = new ScopedEventSubscriber(m_transformComponent->OnTransformChanged,
		std::bind(&MeshRendererBehaviour::UpdateMeshInstanceModel, this));

	m_meshData = _mesh;

	std::shared_ptr<Image> firstTexture;
	std::vector<std::shared_ptr<Image>> textures;
	if (!ResourceManager::TryGetResource<Image>("barstool_albedo", firstTexture))
	{
		LOG_ERROR("Failed to load texture barstool_albedo.png from ResourceManager.");
		throw std::runtime_error("Failed to load texture barstool_albedo.png from ResourceManager.");
	}

	textures.push_back(firstTexture);

	m_instanceId = MoonlitEngine::GetInstance()->Renderer->AddMeshInstance(_mesh, textures, m_transformComponent->GetModelMat());
}

MeshRendererBehaviour::~MeshRendererBehaviour()
{
	delete m_transformChangedSubscriber;
}

std::vector<ParameterRepositoryEntry> MeshRendererBehaviour::GetParameterEntries()
{
	std::vector<ParameterRepositoryEntry> entries = ObjectBehaviour::GetParameterEntries();
	entries.push_back(ParameterRepositoryEntry{
		"Model",
		typeid(m_meshData).name(),
		sizeof(std::shared_ptr<MeshData>),
		&m_meshData
		});

	return entries;
}

void MeshRendererBehaviour::ParameterChanged(const ParameterRepositoryEntry& _parameter)
{
	if (_parameter.Name == "Model")
	{
		if (m_meshData == nullptr)
		{
			LOG_WARNING("MeshRendererBehaviour has no valid mesh assigned.");
			return;
		}

		MoonlitEngine::GetInstance()->Renderer->UpdateInstanceMesh(m_instanceId, m_meshData);
	}
	else if (_parameter.Name == "Transform")
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
	MoonlitEngine::GetInstance()->Renderer->UpdateInstanceModel(m_instanceId, m_transformComponent->GetModelMat());
}
