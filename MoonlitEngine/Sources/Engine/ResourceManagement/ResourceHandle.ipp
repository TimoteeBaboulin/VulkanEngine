#ifndef MOONLIT_RESOURCEHANDLE_IPP
#define MOONLIT_RESOURCEHANDLE_IPP

#include "ResourceHandle.h"
#include "ResourceManager.h"

#include <sstream>

namespace Moonlit::ResourceManagement
{
    template<typename RESOURCE_TYPE>
    ResourceHandle<RESOURCE_TYPE>::BANK_TYPE & ResourceHandle<RESOURCE_TYPE>::GetBank(std::string _handle)
    {
        std::stringstream stream(_handle);
        std::string inputStr;
        std::getline(stream, inputStr, ':');

        return ResourceManager::Instance()->GetBank<BANK_TYPE>(inputStr);
    }

    template<typename RESOURCE_TYPE>
    ResourceHandle<RESOURCE_TYPE>::ResourceHandle(BANK_TYPE &_bank, size_t _index) : m_resourceBank(_bank), m_index(_index)
    {
        m_handle = typeid(RESOURCE_TYPE).name();
    }

    template<typename RESOURCE_TYPE>
    ResourceHandle<RESOURCE_TYPE>::ResourceHandle(std::string& _handle) : m_handle(_handle), m_resourceBank(GetBank(_handle))
    {
        std::stringstream stream(_handle);

        std::string inputStr;
        std::getline(stream, inputStr, ':');
        // Skip the bank line

        std::getline(stream, inputStr, ':');
        m_index = std::stoi(inputStr);
    }

    template<typename RESOURCE_TYPE>
    RESOURCE_TYPE & ResourceHandle<RESOURCE_TYPE>::operator*()
    {
        return m_resourceBank[m_index];
    }

    template<typename RESOURCE_TYPE>
    RESOURCE_TYPE * ResourceHandle<RESOURCE_TYPE>::operator->()
    {
        return &m_resourceBank[m_index];
    }
}

#endif //MOONLIT_RESOURCEHANDLE_IPP