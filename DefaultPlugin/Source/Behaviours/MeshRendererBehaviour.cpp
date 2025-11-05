#include "pch.h"

#include "Behaviours/MeshRendererBehaviour.h"
#include "Behaviours/TransformBehaviour.h"

#include "Engine/GameObject.h"
#include "Engine/MoonlitEngine.h"
#include "Engine/Renderer/Renderer.h"

#include "ResourceManagement/ResourceManager.h"
#include "Debug/Logger.h"
#include "ResourceManagement/Mesh.h"

MeshRendererBehaviour::MeshRendererBehaviour(GameObject* _owner) : ObjectBehaviour(_owner)
{
	LookForTransformComponent();
}

MeshRendererBehaviour::MeshRendererBehaviour(GameObject* _owner, std::shared_ptr<MeshData> _mesh) : ObjectBehaviour(_owner)
{
	LookForTransformComponent();
	m_meshInstance.MeshData = _mesh.get();
	std::shared_ptr<Image> firstTexture;
	if (ResourceManager::TryGetResource<Image>("barstool_albedo", firstTexture))
	{
		m_meshInstance.Textures.push_back(firstTexture);
	}
	else
	{
		LOG_ERROR("Failed to load texture barstool_albedo.png from ResourceManager.");
	}

	MoonlitEngine::GetInstance()->Renderer->AddMeshInstance(m_meshInstance);
}

void MeshRendererBehaviour::LookForTransformComponent()
{
	TransformBehaviour* transform;
	if (!m_owner->TryGetComponentOfType<TransformBehaviour>(transform))
	{
		transform = new TransformBehaviour(m_owner);
	}

	m_meshInstance.Model = *(transform->GetSharedPtrModelMat());
}