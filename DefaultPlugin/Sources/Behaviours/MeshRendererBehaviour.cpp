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
	: ObjectBehaviour(_owner), m_meshData("MeshData")
{
	LookForTransformComponent();

	m_transformChangedSubscriber =
		new Moonlit::Events::ScopedEventSubscriber(m_transformComponent->OnTransformChanged,
		std::bind(&MeshRendererBehaviour::UpdateMeshInstanceModel, this));

	Moonlit::ResourceManagement::ResourceManager* resourceManager = Moonlit::ResourceManagement::ResourceManager::Instance();
	Moonlit::Renderer::MeshHandle meshHandle;

	if (!resourceManager->TryGetResource<MeshData>("Cube", meshHandle))
	{
		LOG_ERROR("Failed to load default mesh barstool_mesh from ResourceManager.");
		throw std::runtime_error("Failed to load default mesh barstool_mesh from ResourceManager.");
	}

	Moonlit::Renderer::TextureHandle textureHandle;
	if (!Moonlit::ResourceManagement::ResourceManager::TryGetResource<Image>("barstool_albedo", textureHandle))
	{
		LOG_ERROR("Failed to load texture barstool_albedo.png from ResourceManager.");
		throw std::runtime_error("Failed to load texture barstool_albedo.png from ResourceManager.");
	}

	std::vector<Moonlit::Renderer::TextureHandle> textures;
	textures.push_back(textureHandle);
	m_meshData = meshHandle;

	m_instanceId = Moonlit::MoonlitEngine::GetInstance()->Renderer->AddMeshInstance(*m_meshData, textures, m_transformComponent->GetModelMat());
}

MeshRendererBehaviour::MeshRendererBehaviour(Moonlit::GameObject* _owner, Moonlit::Renderer::MeshHandle _mesh)
	: ObjectBehaviour(_owner), m_meshData("MeshData", _mesh)
{
	LookForTransformComponent();

	m_transformChangedSubscriber = new Moonlit::Events::ScopedEventSubscriber(m_transformComponent->OnTransformChanged,
	                                                                          std::bind(&MeshRendererBehaviour::UpdateMeshInstanceModel, this));

	m_meshData = _mesh;

	Moonlit::Renderer::TextureHandle firstTexture;
	std::vector<Moonlit::Renderer::TextureHandle> textures;
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
	Moonlit::MoonlitEngine::GetInstance()->Renderer->RemoveMeshInstance(m_instanceId);

	LOG_INFO("MeshRendererBehaviour destroyed.");

	delete m_transformChangedSubscriber;
}

std::vector<ParameterBase*> MeshRendererBehaviour::GetParameters()
{
	std::vector<ParameterBase*> entries = ObjectBehaviour::GetParameters();
	entries.push_back(&m_meshData);

	return entries;
}

void MeshRendererBehaviour::ParameterChanged(const Moonlit::ParameterRepositoryEntry& _parameter)
{
	LOG_INFO("Trying to change parameter " + std::string(_parameter.Name));
	if (strcmp(_parameter.Name, "Model") == 0)
	{
		if ((*m_meshData).ResourcePtr() == nullptr)
		{
			LOG_WARNING("MeshRendererBehaviour has no valid mesh assigned.");
			return;
		}

		Moonlit::MoonlitEngine::GetInstance()->Renderer->UpdateInstanceMesh(m_instanceId, *m_meshData);
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
