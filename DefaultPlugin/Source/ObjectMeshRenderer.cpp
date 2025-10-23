#include "pch.h"

#include "ObjectMeshRenderer.h"
#include "Engine/Components/ObjectTransform.h"

#include "Engine/GameObject.h"
#include "Engine/MoonlitEngine.h"

#include "ResourceManagement/ResourceManager.h"
#include "Debug/Logger.h"
#include "ResourceManagement/Mesh.h"

ObjectMeshRenderer::ObjectMeshRenderer(GameObject* _owner) : ObjectBehaviour(_owner)
{
	LookForTransformComponent();
}

ObjectMeshRenderer::ObjectMeshRenderer(GameObject* _owner, std::shared_ptr<MeshData> _mesh) : ObjectBehaviour(_owner)
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

	MoonlitEngine::GetInstance()->AddMeshInstance(m_meshInstance);
}

void ObjectMeshRenderer::LookForTransformComponent()
{
	ObjectTransform* transform;
	if (!m_owner->TryGetComponentOfType<ObjectTransform>(transform))
	{
		transform = new ObjectTransform(m_owner);
	}

	m_meshInstance.Model = *(transform->GetSharedPtrModelMat());
}