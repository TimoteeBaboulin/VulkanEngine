#pragma once

#include <string>
#include <map>
#include <typeinfo>
#include <typeindex>
#include <vector>
#include "MoonlitExport.h"

namespace Moonlit::ResourceManagement {
    template<typename RESOURCE_TYPE> class ResourceBank;
    template<typename RESOURCE_TYPE> class ResourceHandle;

    /// <summary>
    /// Static class handling ResourceBank interface for easy loading of different resources
    /// </summary>
    class MOONLIT_API ResourceManager {
        // Static
    public:
        template<class RESOURCE_TYPE>
        static void RegisterResourceBank(ResourceBank<RESOURCE_TYPE> *_bank)
        {
            Get().m_resourceBanks[std::type_index(typeid(RESOURCE_TYPE))] = _bank;
        }

        template<class RESOURCE_TYPE>
        static bool TryGetResource(std::string _name, ResourceHandle<RESOURCE_TYPE>& _outResource)
        {
            return Get().TryGetResourceInternal<RESOURCE_TYPE>(_name, _outResource);
        }

        template<class RESOURCE_TYPE>
        static bool TryLoadResource(std::string _path)
        {
            return Get().TryLoadResourceInternal<RESOURCE_TYPE>(_path);
        }

        template<class BANK_TYPE>
        BANK_TYPE *GetBank(std::string _bankName)
        // _bankName is not used yet but it's planned to be used to discriminate between different banks for similar resources
        {
            auto it = m_resourceBanks.find(std::type_index(typeid(BANK_TYPE)));
            if (it != m_resourceBanks.end()) {
                return static_cast<BANK_TYPE *>(it->second);
            }

            return nullptr;
        }

        template<class RESOURCE_TYPE>
        std::vector<ResourceHandle<RESOURCE_TYPE> > GetAllResourcesOfType() {
            std::vector<ResourceHandle<RESOURCE_TYPE> > resources;
            auto it = m_resourceBanks.find(std::type_index(typeid(RESOURCE_TYPE)));
            if (it != m_resourceBanks.end()) {
                ResourceBank<RESOURCE_TYPE> *bank = static_cast<ResourceBank<RESOURCE_TYPE> *>(it->second);
                return bank->GetAllResources();
            }
            return resources;
        }

        static ResourceManager& Get();

        // Instance
    public:
        ResourceManager();

    private:
        template<class RESOURCE_TYPE>
        bool TryGetResourceInternal(std::string _name, ResourceHandle<RESOURCE_TYPE> &_outResource) {
            auto it = m_resourceBanks.find(std::type_index(typeid(RESOURCE_TYPE)));
            if (it != m_resourceBanks.end()) {
                ResourceBank<RESOURCE_TYPE> *bank = static_cast<ResourceBank<RESOURCE_TYPE> *>(it->second);
                _outResource = bank->Get(_name);
                return _outResource.IsValid();
            }

            return false;
        }

        template<class RESOURCE_TYPE>
        bool TryLoadResourceInternal(std::string _path);

        void LoadFileResource(std::string _filepath);

        //Link the resource type to its bank
        std::map<::std::type_index, void *> m_resourceBanks;
    };
}

#include "ResourceManager.ipp"
