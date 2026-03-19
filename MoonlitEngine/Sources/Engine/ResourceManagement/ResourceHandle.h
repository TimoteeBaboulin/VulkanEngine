#ifndef MOONLIT_RESOURCEHANDLE_H
#define MOONLIT_RESOURCEHANDLE_H
#include <string>

#include "ResourceBank.h"

namespace Moonlit::ResourceManagement
{
    template <typename RESOURCE_TYPE>
    class ResourceHandle {
        using BANK_TYPE = ResourceBank<RESOURCE_TYPE>;
        using PAIR_TYPE = BANK_TYPE::PAIR_TYPE;

        static BANK_TYPE& GetBank(std::string _handle);

    public:
        ResourceHandle(BANK_TYPE& _bank, size_t _index);
        ResourceHandle(std::string& _handle);
        RESOURCE_TYPE& operator*();
        RESOURCE_TYPE* operator->();

    private:
        BANK_TYPE& m_resourceBank;
        size_t m_index;

        std::string m_handle;
    };
}


#endif //MOONLIT_RESOURCEHANDLE_H