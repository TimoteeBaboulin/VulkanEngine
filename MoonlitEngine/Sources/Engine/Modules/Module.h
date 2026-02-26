#ifndef MOONLIT_PLUGIN_H
#define MOONLIT_PLUGIN_H

#include "Engine/Component/BehaviourRegistry.h"

#include <windows.h>

namespace Moonlit {
    struct ModuleBehaviourRegistry {
        const char* PluginName;
        BehaviourRegistryEntry *Entries;
        size_t EntryCount;
    };

    struct ModuleMetadata {
        const char* Name;
        const char* Version;
        const char* Author;
        const char* Description;
        const char* FilePath;
    };

    struct ModuleInfo {
        ModuleMetadata Metadata;
        ModuleBehaviourRegistry BehaviourRegistry;
        HMODULE ModuleHandle;
    };
}

#endif //MOONLIT_PLUGIN_H