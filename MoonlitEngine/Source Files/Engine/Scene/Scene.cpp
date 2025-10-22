#include "Engine/Scene/Scene.h"
#include <glm/vec3.hpp>
#include "Engine/GameObject.h"

Scene::Scene()
{
	GameObject* testObject = GameObject::CreateAt(glm::vec3(0, 0, 0));


}

Scene::~Scene()
{
}
