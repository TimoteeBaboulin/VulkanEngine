#ifndef MOONLIT_RESOURCEHANDLE_IPP
#define MOONLIT_RESOURCEHANDLE_IPP

#include "ResourceHandle.h"
#include "ResourceManager.h"

#include <sstream>

namespace Moonlit::ResourceManagement
{
    // template<typename RESOURCE_TYPE>
    // ResourceHandle<RESOURCE_TYPE>::BANK_TYPE & ResourceHandle<RESOURCE_TYPE>::GetBank(std::string _handle)
    // {
    //     std::stringstream stream(_handle);
    //     std::string inputStr;
    //     std::getline(stream, inputStr, ':');
    //
    //     return ResourceManager::Instance()->GetBank<BANK_TYPE>(inputStr);
    // }

    template<typename RESOURCE_TYPE>
    ResourceHandle<RESOURCE_TYPE>::ResourceHandle() : m_resourceBank(nullptr), m_index(-1), m_isValid(false)
    {

    }

    template<typename RESOURCE_TYPE>
    ResourceHandle<RESOURCE_TYPE>::ResourceHandle(BANK_TYPE* _bank, size_t _index) : m_resourceBank(_bank), m_index(_index), m_isValid(m_index != -1)
    {
        m_handle = typeid(RESOURCE_TYPE).name();
    }

    template<typename RESOURCE_TYPE>
    ResourceHandle<RESOURCE_TYPE>::ResourceHandle(std::string& _handle) : m_handle(_handle)
    {
        std::stringstream stream(_handle);

        std::string inputStr;
        std::getline(stream, inputStr, ':');

        m_resourceBank = ResourceManager::Instance()->GetBank<BANK_TYPE>(inputStr);

        std::getline(stream, inputStr, ':');
        m_index = std::stoi(inputStr);

        m_isValid = true;
    }

    template<typename RESOURCE_TYPE>
    RESOURCE_TYPE& ResourceHandle<RESOURCE_TYPE>::operator*()
    {
        return m_resourceBank[m_index].Resource;
    }

    template<typename RESOURCE_TYPE>
    RESOURCE_TYPE* ResourceHandle<RESOURCE_TYPE>::operator->()
    {
        return &m_resourceBank[m_index].Resource;
    }

    template<typename RESOURCE_TYPE>
    ResourceHandle<RESOURCE_TYPE>::operator typename ResourceBank<RESOURCE_TYPE>::PAIR_TYPE()
    {
        return m_resourceBank[m_index];
    }
}

#endif //MOONLIT_RESOURCEHANDLE_IPP