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

	m_transformChangedSubscriber = new ScopedEventSubscriber<glm::mat4>(m_transformComponent->OnTransformChanged,
		std::bind(&MeshRendererBehaviour::UpdateMeshInstanceModel, this, std::placeholders::_1));

	std::shared_ptr<MeshData> defaultMesh;
	ResourceManager* m_manager = ResourceManager::Get();
	if (!m_manager->TryGetResource<MeshData>("barstool", defaultMesh))
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

	m_instanceId = MoonlitEngine::GetInstance()->Renderer->AddMeshInstance(defaultMesh, textures, m_transformComponent->GetModelMat());
}

MeshRendererBehaviour::MeshRendererBehaviour(GameObject* _owner, std::shared_ptr<MeshData> _mesh) : ObjectBehaviour(_owner)
{
	LookForTransformComponent();

	m_transformChangedSubscriber = new ScopedEventSubscriber<glm::mat4>(m_transformComponent->OnTransformChanged,
		std::bind(&MeshRendererBehaviour::UpdateMeshInstanceModel, this, std::placeholders::_1));

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

void MeshRendererBehaviour::LookForTransformComponent()
{
	TransformBehaviour* transform;
	if (!m_owner->TryGetBehaviourOfType<TransformBehaviour>(transform))
	{
		transform = new TransformBehaviour(m_owner);
	}

	m_transformComponent = transform;
}

void MeshRendererBehaviour::UpdateMeshInstanceModel(glm::mat4 _model)
{
	MoonlitEngine::GetInstance()->Renderer->UpdateInstanceModel(m_instanceId, _model);
}
