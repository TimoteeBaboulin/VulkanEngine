#pragma once

#include "ResourceHandle.h"
#include "ResourceBank.h"

namespace Moonlit::ResourceManagement
{
    template<class BANK_TYPE>
    void ResourceManager::InitBank()
    {
        BANK_TYPE::Initialize();
        m_resourceBanks[std::type_index(typeid(typename BANK_TYPE::resource_type))] = (void*)&BANK_TYPE::Get();
    }

    template<class RESOURCE_TYPE>
    bool ResourceManager::TryLoadResourceInternal(std::string _path) {
        auto it = m_resourceBanks.find(std::type_index(typeid(RESOURCE_TYPE)));
        if (it != m_resourceBanks.end()) {
            auto *bank = static_cast<ResourceBank<RESOURCE_TYPE> *>(it->second);
            return bank->TryLoad(_path);
        }

        return false;
    }
}

