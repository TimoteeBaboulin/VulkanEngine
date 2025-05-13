#pragma once

#include <string>
#include <vector>

#include <filesystem>

constexpr int ResourceBankDefaultSize = 10;

template <class T>
struct ResourcePair
{
	std::string Name;
	T Resource;
};

template <class T>
class ResourceBank
{
public:
	static ResourceBank<T>* Instance;

	ResourceBank();
	T* Get(std::string _name);
	virtual bool TryLoad(std::string _filepath) = 0;
	bool Exist(std::string _name);

protected:
	std::vector<ResourcePair<T>> m_resources;
};

template<class T>
inline ResourceBank<T>::ResourceBank() : m_resources(ResourceBankDefaultSize)
{
}

template<class T>
inline T* ResourceBank<T>::Get(std::string _name)
{
	for (auto it = m_resources.begin(); it != m_resources.end(); it++)
	{
		if ((*it).Name == _name)
		{
			return &(*it).Resource;
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
