#include "Engine/Scene/Scene.h"
#include <glm/vec3.hpp>
#include "Engine/GameObject.h"

#include "Debug/Logger.h"

//TODO: This is only used for testing, remove it later
#include "Engine/Components/ObjectMeshRenderer.h"
#include "ResourceManagement/ResourceManager.h"

Scene::Scene()
{
	GameObject* testObject = GameObject::CreateAt(glm::vec3(1, 0, 0));
	std::shared_ptr<MeshData> testMesh;
	if (ResourceManager::TryGetResource<MeshData>("barstool", testMesh))
	{
		ObjectMeshRenderer* meshRenderer = new ObjectMeshRenderer(testObject, testMesh);
		//LOG_INFO("Barstool mesh successfully queried from the resource manager");
	}
	else
	{
		LOG_WARNING("Couldn't query the barstool mesh from the resource manager");
	}
}

Scene::~Scene()
{
	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
	{
		delete (*it);
	}
}
