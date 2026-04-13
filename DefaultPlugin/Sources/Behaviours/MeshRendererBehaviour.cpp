#include "pch.h"

#include "MeshRendererBehaviour.h"
#include "TransformBehaviour.h"

#include "Engine/Component/GameObject.h"
#include "Engine/MoonlitEngine.h"
#include "Engine/Renderer/Renderer.h"

#include "Engine/ResourceManagement/ResourceManager.h"
#include "Engine/ResourceManagement/ResourceHandle.h"
#include "Debug/Logger.h"

using Image = Moonlit::Image;
using MeshData = Moonlit::MeshData;

MeshRendererBehaviour::MeshRendererBehaviour(Moonlit::GameObject* _owner)
	: ObjectBehaviour(_owner), m_meshData("MeshData")
{
	LOG_INFO("[MeshRendererBehaviour] Constructor called for GameObject");
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

	Moonlit::Renderer::TextureHandle textureHandle;
	if (!Moonlit::ResourceManagement::ResourceManager::TryGetResource<Image>("barstool_albedo", textureHandle))
	{
		LOG_ERROR("Failed to load texture barstool_albedo.png from ResourceManager.");
		throw std::runtime_error("Failed to load texture barstool_albedo.png from ResourceManager.");
	}

	MeshData& data = *meshHandle;

	std::vector<Moonlit::Renderer::TextureHandle> textures;
	textures.push_back(textureHandle);
	m_meshData = meshHandle;

	m_instanceId = Moonlit::MoonlitEngine::Get().Renderer->AddMeshInstance(*m_meshData, textures, m_transformComponent->GetModelMat());
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

	m_instanceId = Moonlit::MoonlitEngine::Get().Renderer->AddMeshInstance(_mesh, textures, m_transformComponent->GetModelMat());
}

MeshRendererBehaviour::~MeshRendererBehaviour()
{
	Moonlit::MoonlitEngine::Get().Renderer->RemoveMeshInstance(m_instanceId);

	LOG_INFO("MeshRendererBehaviour destroyed.");

	delete m_transformChangedSubscriber;
}

std::vector<ParameterBase*> MeshRendererBehaviour::GetParameters()
{
	std::vector<ParameterBase*> entries = ObjectBehaviour::GetParameters();
	entries.push_back(&m_meshData);

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

		if ((*m_meshData).IsValid()) {
			Moonlit::MoonlitEngine::Get().Renderer->UpdateInstanceMesh(m_instanceId, *m_meshData);
		}
	}
}

void MeshRendererBehaviour::LoadFromFile(nlohmann::json &_stream)
{
	ObjectBehaviour::LoadFromFile(_stream);

	if ((*m_meshData).IsValid()) {
		Moonlit::MoonlitEngine::Get().Renderer->UpdateInstanceMesh(m_instanceId, *m_meshData);
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
	Moonlit::MoonlitEngine::Get().Renderer->UpdateInstanceModel(m_instanceId, m_transformComponent->GetModelMat());
}
