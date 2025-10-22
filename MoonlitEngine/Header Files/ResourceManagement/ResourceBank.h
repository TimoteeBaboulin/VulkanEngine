#pragma once

#include <string>
#include <vector>
#include <memory>

constexpr int ResourceBankDefaultSize = 10;

//Forward declaration
template <class T>
class ResourceBank;

template <class T>
struct ResourcePair
{
	std::string Name;
	std::shared_ptr<T> Resource;
};

template <class T>
class ResourceBank
{
public:
	static inline ResourceBank<T>* Instance;

	ResourceBank();
	std::shared_ptr<T> Get(std::string _name);
	virtual bool TryLoad(std::string _filepath) = 0;
	bool Exist(std::string _name);
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
			return (*it).Resource;
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
		if ((*it).Resource.use_count() == 1)
		{
			//TODO: Handle resource unloading properly
		}
	}
}
