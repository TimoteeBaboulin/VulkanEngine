#pragma once

#include <stdexcept>
#include "ResourceHandle.h"

namespace Moonlit::ResourceManagement
{
    template<class T>
    ResourceBank<T>::ResourceBank() : m_resources()
    {
    }

    template<class RESOURCE_TYPE>
    std::vector<typename ResourceBank<RESOURCE_TYPE>::HANDLE_TYPE> ResourceBank<RESOURCE_TYPE>::GetAllResources()
    {
        std::vector<HANDLE_TYPE> result;
        std::lock_guard<std::mutex> lock(m_mutex);

        result.reserve(m_resources.size());
        for (const auto& [name, _] : m_resources)
        {
            result.push_back(HANDLE_TYPE(this, name));
        }

        return result;
    }

    template<class RESOURCE_TYPE>
    std::vector<std::string> ResourceBank<RESOURCE_TYPE>::GetAllNames() {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::vector<std::string> result;

        for (const auto& [name, _] : m_resources)
        {
            result.push_back(name);
        }

        return result;
    }

    template<class RESOURCE_TYPE>
    ResourceHandle<RESOURCE_TYPE> ResourceBank<RESOURCE_TYPE>::Get(std::string _name)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (m_resources.contains(_name)) {
            return HANDLE_TYPE(this, _name);
        }

        return HANDLE_TYPE(this, "");
    }

    template<class T>
    bool ResourceBank<T>::Exist(std::string _name)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_resources.contains(_name);
    }

    template<class T>
    T& ResourceBank<T>::operator[](std::string _name) {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_resources.contains(_name))
        {
            throw std::out_of_range("ResourceBank::operator[]");
        }

        return m_resources[_name];
    }

    template<class T>
    void ResourceBank<T>::TryUnloadUnusedResources()
    {
        // for (auto it = m_resources.begin(); it != m_resources.end(); it++)
        // {
        //     if ((*it).ResourcePtr.use_count() == 1)
        //     {
        //         //TODO: Handle resource unloading properly
        //         m_resources.erase(it);
        //     }
        // }
    }

    template<class RESOURCE_TYPE>
    void ResourceBank<RESOURCE_TYPE>::InsertResource(const std::string &_name, const RESOURCE_TYPE& _resource) {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (m_resources.contains(_name)) {
            LOG_ERROR("Resource with name " + _name + " already exist in ResourceBank.");
        }

        m_resources[_name] = _resource;
    }
}

