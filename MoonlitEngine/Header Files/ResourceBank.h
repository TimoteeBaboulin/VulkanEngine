#pragma once

#include <string>
#include <vector>

#include <filesystem>

template <class T>
class ResourceBank
{
public:
	static ResourceBank<T>* Instance;

	ResourceBank(T(*_loader)(std::string));
	T* Get(std::string _name);
	bool Load(std::string _filepath);
	bool Exist(std::string _name);

private:
	struct ResourcePair<T>
	{
		std::string Name;
		T Resource;
	};

	std::vector<ResourcePair<T>> m_resources;
	T(*m_loadingMethod)(std::string);
};

template<class T>
inline ResourceBank<T>::ResourceBank(T(*_loader)(std::string))
{
	m_loadingMethod = _loader;
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
inline bool ResourceBank<T>::Load(std::string _filepath)
{
	std::filesystem::path path = new std::filesystem::path(_filepath);
	if (path.empty())
		return false;

	std::string resourceName = path.stem();
	if (Exist(resourceName))
		return false;

	try
	{
		m_resources.push_back(ResourceBank{
		.Name = resourceName,
		.Resource = m_loadingMethod(_filepath)
			});
	}
	catch (std::exception err)
	{
		std::cout << err.what() << std::endl;
		return false;
	}
	return true;
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
