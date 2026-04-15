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
	: ObjectBehaviour(_owner), m_meshData("MeshData"), m_textureHandles("TextureHandles")
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

	Moonlit::Renderer::TextureHandle textureHandle;
	if (!Moonlit::ResourceManagement::ResourceManager::TryGetResource<Image>("barstool_albedo", textureHandle))
	{
		LOG_ERROR("Failed to load texture barstool_albedo.png from ResourceManager.");
		throw std::runtime_error("Failed to load texture barstool_albedo.png from ResourceManager.");
	}

	std::vector<Moonlit::Renderer::TextureHandle> textures;
	textures.push_back(textureHandle);
	MeshData& data = *meshHandle;

	m_meshData = meshHandle;
	m_textureHandles = textures;
}

MeshRendererBehaviour::MeshRendererBehaviour(Moonlit::GameObject* _owner, Moonlit::Renderer::MeshHandle _mesh)
	: MeshRendererBehaviour(_owner)
{
	m_meshData = _mesh;
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
	entries.push_back(&m_textureHandles);

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
}

void MeshRendererBehaviour::Init()
{
	ObjectBehaviour::Init();

	m_instanceId = Moonlit::MoonlitEngine::Get().Renderer->AddMeshInstance(*m_meshData, *m_textureHandles, m_transformComponent->GetModelMat());
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
