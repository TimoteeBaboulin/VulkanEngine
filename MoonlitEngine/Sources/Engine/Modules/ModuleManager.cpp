#include "ModuleManager.h"
#include "../../../Sources/Debug/Logger.h"

#include <filesystem>
#include <windows.h>

Moonlit::ModuleManager::ModuleManager() {
    m_loadedModules.reserve(DefaultModuleReserveSize);
}

Moonlit::ModuleManager::~ModuleManager() {
    UnloadAllModules();
}

void Moonlit::ModuleManager::LoadModulesFromDirectory(std::string _directory) {
    std::filesystem::path pluginDir(_directory);
    if (!std::filesystem::exists(pluginDir) || !std::filesystem::is_directory(pluginDir)) {
        throw std::invalid_argument("Directory does not exist");
    }

    for (auto it = std::filesystem::directory_iterator(pluginDir); it != std::filesystem::directory_iterator(); ++it) {
        if (it->is_regular_file() && it->path().extension() == ".dll") {
            LoadModule(it->path());
        }
    }
}

void Moonlit::ModuleManager::UnloadAllModules() {
    // Code to unload all currently loaded plugins
    for (auto it = m_loadedModules.begin(); it != m_loadedModules.end(); ++it) {
        FreeLibrary(it->ModuleHandle);
    }

    m_loadedModules.clear();
}

void Moonlit::ModuleManager::UnloadModule(std::string _name) {
    auto it = std::find_if(m_loadedModules.begin(), m_loadedModules.end(), [&_name](const ModuleInfo& plugin) {
        return plugin.Metadata.Name == _name;
    });

    if (it != m_loadedModules.end()) {
        FreeLibrary(it->ModuleHandle);
        m_loadedModules.erase(it);
    } else {
        LOG_WARNING("Module with name " + _name + " not found, cannot unload.");
    }
}

void Moonlit::ModuleManager::LoadModule(std::filesystem::path _path) {
    HMODULE moduleHandle = LoadLibraryA(_path.string().c_str());
    if (moduleHandle == 0) {
        DWORD error = GetLastError();
        throw std::runtime_error("Failed to load module: " + _path.string() + " Error: " + std::to_string(error));
    }

    FARPROC registerFunction = GetProcAddress(moduleHandle, "GetMetadata");
    if (registerFunction == 0) {
        FreeLibrary(moduleHandle);
        LOG_INFO(_path.string() + " does not contain GetMetadata function, it's probably not a module.");
        return;
    }

    auto getMetadataFunc = reinterpret_cast<ModuleMetadata(*)()>(registerFunction);
    ModuleMetadata metadata = getMetadataFunc();

    if (metadata.Name == nullptr) {
        FreeLibrary(moduleHandle);
        throw std::runtime_error("Module " + _path.string() + " has invalid metadata: Name is null.");
    }

    registerFunction = GetProcAddress(moduleHandle, "GetRegistry");
    auto getBehaviourFunc = reinterpret_cast<ModuleBehaviourRegistry(*)()>(registerFunction);
    ModuleBehaviourRegistry moduleBehaviourRegistry = getBehaviourFunc();

    ModuleInfo moduleInfo;
    moduleInfo.Metadata = metadata;
    moduleInfo.BehaviourRegistry = moduleBehaviourRegistry;
    moduleInfo.ModuleHandle = moduleHandle;

    for (int i = 0; i < moduleBehaviourRegistry.EntryCount; i++) {
        LOG_INFO("Registering behaviour: " + std::string(moduleBehaviourRegistry.Entries[i].Name) + " from module: " + std::string(metadata.Name));
        BehaviourRegistry::RegisterBehaviour(moduleBehaviourRegistry.Entries[i].Name,
            moduleBehaviourRegistry.Entries[i].CreateFunction);
    }

    LOG_INFO("Successfully loaded module: " + std::string(metadata.Name) + " Version: " + std::string(metadata.Version));
    m_loadedModules.push_back(moduleInfo);
}