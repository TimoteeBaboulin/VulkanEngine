#pragma once

#include <vector>
#include <string>

class Scene
{
public:
	Scene();
	~Scene();

	void Save(std::string _filePath);
	void Load(std::string _filePath);

	void ClearScene();
private:
	std::vector<class GameObject*> m_gameObjects;
};