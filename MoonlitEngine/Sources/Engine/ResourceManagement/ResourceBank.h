#pragma once

#include <string>
#include <vector>
#include <memory>

namespace Moonlit::ResourceManagement
{
	constexpr int ResourceBankDefaultSize = 10;
	//
	////Forward declaration
	//template <class T>
	//class ResourceBank;

	template <class T>
	struct ResourcePair
	{
		std::string Name;
		T Resource;
		std::shared_ptr<T> ResourcePtr;
	};

	/// <summary>
	/// Generic class holding the loaded resources and implementing load operations 
	/// </summary>
	template <class T>
	class ResourceBank
	{
	public:
		static inline ResourceBank<T>* Instance;

		ResourceBank();

		// Getters

		std::vector<ResourcePair<T>> GetAllResources() { return m_resources; }
		std::shared_ptr<T> Get(std::string _name);
		bool Exist(std::string _name);

		// Load/Unload

		virtual bool TryLoad(std::string _filepath) = 0;
		void TryUnloadUnusedResources();

	protected:
		std::vector<ResourcePair<T>> m_resources;
	};

	template<class T>
	inline ResourceBank<T>::ResourceBank() : m_resources()
	{
		m_resources.reserve(ResourceBankDefaultSize);
	}

	template<class T>
	inline std::shared_ptr<T> ResourceBank<T>::Get(std::string _name)
	{
		for (auto it = m_resources.begin(); it != m_resources.end(); it++)
		{
			if ((*it).Name == _name)
			{
				return (*it).ResourcePtr;
			}
		}

		return nullptr;
	}

	template<class T>
	inline bool ResourceBank<T>::Exist(std::string _name)
	{
		for (auto it = m_resources.begin(); it != m_resources.end(); it++)
		{
			if (_name == (*it).Name)
				return true;
		}
		return false;
	}

	template<class T>
	inline void ResourceBank<T>::TryUnloadUnusedResources()
	{
		for (auto it = m_resources.begin(); it != m_resources.end(); it++)
		{
			if ((*it).ResourcePtr.use_count() == 1)
			{
				//TODO: Handle resource unloading properly
				m_resources.erase(it);
			}
		}
	}
}