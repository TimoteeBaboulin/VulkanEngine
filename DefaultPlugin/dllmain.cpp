// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include "Behaviours/MeshRendererBehaviour.h"
#include "Behaviours/TransformBehaviour.h"

#include "Engine/Components/BehaviourRegistry.h"

#include "Debug/Logger.h"

#include <string>

#define REGISTER_BEHAVIOUR(behaviour, func) \
    entries.push_back(BehaviourRegistryEntry{ ClassNameFromTypeName(typeid(behaviour).name()), func });

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

ObjectBehaviour* CreateMeshRenderer(GameObject* _owner)
{
    return new MeshRendererBehaviour(_owner);
}

ObjectBehaviour* CreateTransform(GameObject* _owner)
{
    return new TransformBehaviour(_owner);
}

/// <summary>
/// This is the library's entry point for registering custom behaviours
/// </summary>
extern "C" __declspec(dllexport) std::vector<BehaviourRegistryEntry> GetRegistry()
{
	std::vector<BehaviourRegistryEntry> entries;

	REGISTER_BEHAVIOUR(MeshRendererBehaviour, CreateMeshRenderer);
	REGISTER_BEHAVIOUR(TransformBehaviour, CreateTransform);

	LOG_INFO("Default Plugin behaviours registered.");
	return entries;
}