#define REGISTER_BEHAVIOUR(behaviour, func) \
entries.push_back(Moonlit::BehaviourRegistryEntry{ Moonlit::ClassNameFromTypeName(typeid(behaviour).name()), func });

#include <windows.h>

#include "UserModuleTestBehaviour.h"
#include "Engine/MoonlitEngine.h"
#include "Engine/Component/ObjectBehaviour.h"
#include "Engine/Modules/Module.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}

Moonlit::ObjectBehaviour* CreateUserBehaviour(Moonlit::GameObject* _owner)
{
    return new UserModuleTestBehaviour(_owner);
}

extern "C" __declspec(dllexport) Moonlit::ModuleMetadata GetMetadata() {
    Moonlit::ModuleMetadata metadata;
    metadata.Name = "Default Plugin";
    metadata.Version = "1.0.0";
    metadata.Description = "The default plugin.";
    metadata.Author = "Moonlit Team";
    return metadata;
}

extern "C" __declspec(dllexport) Moonlit::ModuleBehaviourRegistry GetRegistry()
{
    Moonlit::ModuleBehaviourRegistry registry;
    registry.PluginName = "Default Plugin";
    registry.EntryCount = 1;
    registry.Entries = new Moonlit::BehaviourRegistryEntry[registry.EntryCount];
    registry.Entries[0] = { Moonlit::ClassNameFromTypeName(typeid(UserModuleTestBehaviour).name()), &CreateUserBehaviour };

    return registry;
}