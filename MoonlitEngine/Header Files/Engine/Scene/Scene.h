#pragma once

#include <vector>
#include <string>

#include "MoonlitExport.h"

/// <summary>
/// Represents a level
/// </summary>
class MOONLIT_API Scene
{
public:
	Scene();
	~Scene();

	void Save(std::string _filePath);
	void Load(std::string _filePath);

	void ClearScene();

	void AddGameObject(class GameObject* _gameObject, class GameObject* _parent = nullptr);

	class GameObject* GetGameObjectById(uint64_t _id) const;
	class GameObject* GetGameObjectByName(const std::string& _name) const;

	std::vector<class GameObject*> GetRootGameObjects() const { return m_rootGameObjects; }
private:
	std::vector<class GameObject*> m_rootGameObjects;
};