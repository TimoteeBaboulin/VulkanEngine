#ifndef MOONLIT_RESOURCEBANK_IPP
#define MOONLIT_RESOURCEBANK_IPP

#include <stdexcept>

#include "ResourceBank.h"

namespace Moonlit::ResourceManagement
{
    template<class T>
    ResourceBank<T>::ResourceBank() : m_resources()
    {
        m_resources.reserve(ResourceBankDefaultSize);
    }

    template<class T>
    std::shared_ptr<T> ResourceBank<T>::Get(std::string _name)
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
    bool ResourceBank<T>::Exist(std::string _name)
    {
        for (auto it = m_resources.begin(); it != m_resources.end(); it++)
        {
            if (_name == (*it).Name)
                return true;
        }
        return false;
    }

    template<class T>
    ResourceBank<T>::PAIR_TYPE& ResourceBank<T>::operator[](size_t index) {
        if (m_resources.size() <= index)
        {
            throw std::out_of_range("ResourceBank::operator[]");
        }

        return m_resources[index];
    }

    template<class T>
    void ResourceBank<T>::TryUnloadUnusedResources()
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

#endif //MOONLIT_RESOURCEBANK_IPP