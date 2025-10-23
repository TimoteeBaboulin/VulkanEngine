// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include "PluginBehaviourTest.h"
#include "Engine/Components/BehaviourRegistry.h"

#include "Debug/Logger.h"

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

ObjectBehaviour* CreatePluginBehaviourTest(GameObject* _owner)
{
    return new PluginBehaviourTest(_owner);
}

/// <summary>
/// This is the library's entry point for registering custom behaviours
/// </summary>
/// <returns></returns>
extern "C" __declspec(dllexport) std::vector<BehaviourRegistryEntry> GetRegistry()
{
	std::vector<BehaviourRegistryEntry> entries;
	entries.push_back(BehaviourRegistryEntry{ typeid(PluginBehaviourTest).name(), CreatePluginBehaviourTest });
	LOG_INFO("Default Plugin behaviours registered.");
	return entries;
}