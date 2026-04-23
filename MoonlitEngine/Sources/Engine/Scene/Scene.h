#pragma once

#include <vector>
#include <string>

#include "../../MoonlitExport.h"

namespace Moonlit
{

	/// <summary>
	/// Represents a level
	/// </summary>
	class MOONLIT_API Scene
	{
	public:
		Scene();
		Scene(std::string _savePath);
		~Scene();

		void Save(const std::string& _filePath);
		void Load(const std::string& _filePath);

		void ClearScene();

		void AddGameObject(class GameObject* _gameObject, class GameObject* _parent = nullptr);
		void RemoveGameObject(class GameObject* _gameObject);

		void Start();

		void Update(const float& _deltaTime);
		void LateUpdate(const float& _deltaTime);

		void PreRender();
		void PostRender();

		std::string GetSavePath() const {return m_savePath;}

		GameObject* GetGameObjectById(uint64_t _id) const;
		GameObject* GetGameObjectByName(const std::string& _name) const;

		std::vector<class GameObject*> GetRootGameObjects() const { return m_rootGameObjects; }
	private:
		std::vector<class GameObject*> m_rootGameObjects;
		std::string m_savePath;
	};
}