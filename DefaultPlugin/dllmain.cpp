// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include "PluginBehaviourTest.h"
#include "Engine/Components/BehaviourRegistry.h"

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

/// <summary>
/// This is the library's entry point for registering custom behaviours
/// </summary>
/// <returns></returns>
void APIENTRY RegisterBehaviours()
{
    BehaviourRegistry::RegisterBehaviour<PluginBehaviourTest>(
        [](GameObject* _owner)
        {
            new PluginBehaviourTest(_owner);
        }
	);
}