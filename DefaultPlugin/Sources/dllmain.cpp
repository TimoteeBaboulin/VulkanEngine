#include "pch.h"

#include "Behaviours/MeshRendererBehaviour.h"
#include "Behaviours/TransformBehaviour.h"

#include "Engine/Component/BehaviourRegistry.h"
#include "Engine/Modules/Module.h"

#include <string>

#define REGISTER_BEHAVIOUR(behaviour, func) \
    entries.push_back(Moonlit::BehaviourRegistryEntry{ Moonlit::ClassNameFromTypeName(typeid(behaviour).name()), func });

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

Moonlit::ObjectBehaviour* CreateMeshRenderer(Moonlit::GameObject* _owner)
{
    return new MeshRendererBehaviour(_owner);
}

Moonlit::ObjectBehaviour* CreateTransform(Moonlit::GameObject* _owner)
{
    return new TransformBehaviour(_owner);
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
    registry.EntryCount = 2;
    registry.Entries = new Moonlit::BehaviourRegistryEntry[registry.EntryCount];
    registry.Entries[0] = { Moonlit::ClassNameFromTypeName(typeid(MeshRendererBehaviour).name()), CreateMeshRenderer };
    registry.Entries[1] = { Moonlit::ClassNameFromTypeName(typeid(TransformBehaviour).name()), CreateTransform };

	return registry;
}