#pragma once

#include "../ResourceBank.h"
#include "Engine/Renderer/Material/Material.h"
#include "common.h"

namespace Moonlit::ResourceManagement
{
    class MOONLIT_API MaterialBank : public ResourceBank<Moonlit::Material>
    {
    public:
        static void Initialize()
        {
            if (Instance == nullptr)
                Instance = new MaterialBank();
        }

        static MaterialBank& Get()
        {
            return *static_cast<MaterialBank*>(Instance);
        }

        bool TryLoad(const std::string& _filepath) override;
    };
}
