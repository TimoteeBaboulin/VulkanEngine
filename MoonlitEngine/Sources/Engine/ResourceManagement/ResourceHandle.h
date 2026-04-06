#ifndef MOONLIT_RESOURCEHANDLE_H
#define MOONLIT_RESOURCEHANDLE_H

#include <string>
#include <memory>

namespace Moonlit::ResourceManagement
{
    template<typename RESOURCE_TYPE> class ResourceBank;
    template<typename RESOURCE_TYPE> struct ResourcePair;

    template <typename RESOURCE_TYPE>
    class ResourceHandle {
        using BANK_TYPE = ResourceBank<RESOURCE_TYPE>;
        using PAIR_TYPE = ResourcePair<RESOURCE_TYPE>;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(ResourceHandle, m_handle, m_index)

    public:
        ResourceHandle();
        ResourceHandle(BANK_TYPE* _bank, size_t _index);
        ResourceHandle(std::string& _handle);
        RESOURCE_TYPE& operator*();
        RESOURCE_TYPE* operator->();
        operator PAIR_TYPE();

        std::string Name() const;
        std::shared_ptr<RESOURCE_TYPE> ResourcePtr() const;
        bool IsValid() const;
    private:
        BANK_TYPE* m_resourceBank;
        size_t m_index;

        std::string m_handle;
        bool m_isValid = false;
    };
}

#include "ResourceHandle.ipp"

#endif //MOONLIT_RESOURCEHANDLE_H