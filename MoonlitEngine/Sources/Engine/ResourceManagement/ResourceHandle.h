#pragma once

#include <string>
#include <memory>
#include <nlohmann/json.hpp>

namespace Moonlit::ResourceManagement
{
    template<typename RESOURCE_TYPE> class ResourceBank;
    template<typename RESOURCE_TYPE> struct ResourcePair;

    template <typename RESOURCE_TYPE>
    class ResourceHandle {
        using BANK_TYPE = ResourceBank<RESOURCE_TYPE>;
        using PAIR_TYPE = ResourcePair<RESOURCE_TYPE>;

    public:
        ResourceHandle();
        ResourceHandle(BANK_TYPE* _bank, std::string _name);
        ResourceHandle(std::string& _handle);
        RESOURCE_TYPE& operator*();
        RESOURCE_TYPE* operator->();
        operator PAIR_TYPE();

        [[nodiscard]] std::string Name() const;
        [[nodiscard]] std::string Handle() const;
        [[nodiscard]] bool IsValid() const;
    private:
        BANK_TYPE* m_resourceBank;
        std::string m_name;
        std::string m_handle;
    };
}

#include "ResourceHandle.ipp"

