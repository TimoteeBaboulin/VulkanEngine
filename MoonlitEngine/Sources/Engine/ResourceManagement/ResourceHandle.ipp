#pragma once

#include "ResourceManager.h"

#include <sstream>

#include "Debug/Logger.h"

namespace Moonlit::ResourceManagement
{
    template<typename RESOURCE_TYPE>
    ResourceHandle<RESOURCE_TYPE>::ResourceHandle() : m_resourceBank(nullptr), m_name("")
    {
        m_handle = typeid(RESOURCE_TYPE).name();
    }

    template<typename RESOURCE_TYPE>
    ResourceHandle<RESOURCE_TYPE>::ResourceHandle(BANK_TYPE* _bank, std::string _name) : m_resourceBank(_bank), m_name(_name)
    {
        m_handle = typeid(RESOURCE_TYPE).name();
        m_handle += ":" + _name;
    }

    template<typename RESOURCE_TYPE>
    ResourceHandle<RESOURCE_TYPE>::ResourceHandle(std::string& _handle) : m_handle(_handle)
    {
        std::stringstream stream(_handle);

        std::string inputStr;
        std::getline(stream, inputStr, ':');

        m_resourceBank = ResourceManager::Get().GetBank<BANK_TYPE>(inputStr);

        std::getline(stream, m_name, ':');
    }

    template<typename RESOURCE_TYPE>
    RESOURCE_TYPE& ResourceHandle<RESOURCE_TYPE>::operator*()
    {
        if (!IsValid()) {
            LOG_ERROR("Trying to dereference an invalid resource handle: " + m_handle);
            throw std::runtime_error("Invalid resource handle: " + m_handle);
        }

        return (*m_resourceBank)[m_name];
    }

    template<typename RESOURCE_TYPE>
    RESOURCE_TYPE* ResourceHandle<RESOURCE_TYPE>::operator->()
    {
        if (!IsValid()) {
            LOG_ERROR("Trying to access an invalid resource handle: " + m_handle);
            throw std::runtime_error("Invalid resource handle: " + m_handle);
        }

        return &(*m_resourceBank)[m_name];
    }

    template<typename RESOURCE_TYPE>
    ResourceHandle<RESOURCE_TYPE>::operator ResourcePair<RESOURCE_TYPE>() {
        return *this;
    }

    template<typename RESOURCE_TYPE>
    std::string ResourceHandle<RESOURCE_TYPE>::Name() const
    {
        return m_name;
    }

    template<typename RESOURCE_TYPE>
    std::string ResourceHandle<RESOURCE_TYPE>::Handle() const
    {
        return m_handle;
    }

    template<typename RESOURCE_TYPE>
    bool ResourceHandle<RESOURCE_TYPE>::IsValid() const
    {
        return !m_name.empty() && m_resourceBank != nullptr;
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

