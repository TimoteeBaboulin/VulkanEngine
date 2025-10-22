#pragma once

#include <vector>

class Scene
{
public:
	Scene();
	~Scene();

private:
	std::vector<class GameObject*> m_gameObjects;
};