#pragma once

#include <vector>
#include <string>

/// <summary>
/// Represents a level
/// </summary>
class Scene
{
public:
	Scene();
	~Scene();

	void Save(std::string _filePath);
	void Load(std::string _filePath);

	void ClearScene();

	std::vector<class GameObject*> GetRootGameObjects() const { return m_rootGameObjects; }
private:
	std::vector<class GameObject*> m_rootGameObjects;
};