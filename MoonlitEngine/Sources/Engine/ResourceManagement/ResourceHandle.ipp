#ifndef MOONLIT_RESOURCEHANDLE_IPP
#define MOONLIT_RESOURCEHANDLE_IPP

#include "ResourceManager.h"
#include "ResourceBank.h"

#include <sstream>

namespace Moonlit::ResourceManagement
{
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
        PAIR_TYPE& pair = (*m_resourceBank)[m_index];
        return pair.Resource;
    }

    template<typename RESOURCE_TYPE>
    RESOURCE_TYPE* ResourceHandle<RESOURCE_TYPE>::operator->()
    {
        return &(*m_resourceBank)[m_index].Resource;
    }

    template<typename RESOURCE_TYPE>
    ResourceHandle<RESOURCE_TYPE>::operator ResourcePair<RESOURCE_TYPE>() {
        return *this;
    }

    template<typename RESOURCE_TYPE>
    std::string ResourceHandle<RESOURCE_TYPE>::Name() const
    {
        return (*m_resourceBank)[m_index].Name;
    }

    template<typename RESOURCE_TYPE>
    std::string ResourceHandle<RESOURCE_TYPE>::Handle() const
    {
        return m_handle;
    }

    template<typename RESOURCE_TYPE>
    std::shared_ptr<RESOURCE_TYPE> ResourceHandle<RESOURCE_TYPE>::ResourcePtr() const
    {
        return (*m_resourceBank)[m_index].ResourcePtr;
    }

    template<typename RESOURCE_TYPE>
    bool ResourceHandle<RESOURCE_TYPE>::IsValid() const
    {
        return m_isValid;
    }
}

namespace nlohmann {
    template<typename T>
    struct adl_serializer<Moonlit::ResourceManagement::ResourceHandle<T>> {
        static void to_json(json& j, const Moonlit::ResourceManagement::ResourceHandle<T>& h) {
            j = json{{"handle", h.Handle()}};
        }

        static void from_json(const json& j, Moonlit::ResourceManagement::ResourceHandle<T>& h) {
            std::string handle = j.at("handle").get<std::string>();
            h = Moonlit::ResourceManagement::ResourceHandle<T>(handle);
        }
    };
}

#endif //MOONLIT_RESOURCEHANDLE_IPP