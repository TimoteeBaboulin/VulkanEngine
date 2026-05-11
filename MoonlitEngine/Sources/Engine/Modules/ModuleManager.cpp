#include "ModuleManager.h"
#include "Debug/Logger.h"

#include <filesystem>
#include <windows.h>


void Moonlit::ModuleManager::LoadModulesFromDirectory(const std::string& _directory) {
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
    std::lock_guard<std::mutex> lock(m_mutex);

    while (!m_loadedModules.empty()) {
        unloadModuleInternal(m_loadedModules.front().Metadata.Name);
    }
}

void Moonlit::ModuleManager::UnloadModule(std::string _name) {
    std::lock_guard<std::mutex> lock(m_mutex);
    unloadModuleInternal(_name);
}

void Moonlit::ModuleManager::LoadModule(const std::filesystem::path& _path) {
    ModuleInfo moduleInfo;
    if (!TryGetModule(_path, moduleInfo)) {
        LOG_WARNING("Failed to load module from path: " + _path.string());
        return;
    }

    ModuleBehaviourRegistry moduleBehaviourRegistry = moduleInfo.BehaviourRegistry;
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        for (int i = 0; i < moduleBehaviourRegistry.EntryCount; i++) {
            BehaviourRegistry::RegisterBehaviour(moduleBehaviourRegistry.Entries[i].Name,
                moduleBehaviourRegistry.Entries[i].CreateFunction);
        }

        LOG_INFO("Successfully loaded module: " + std::string(moduleInfo.Metadata.Name) + " Version: " + std::string(moduleInfo.Metadata.Version));
        m_loadedModules.push_back(moduleInfo);
    }

}

Moonlit::ModuleManager::ModuleManager() {
    m_loadedModules.reserve(DefaultModuleReserveSize);
}

Moonlit::ModuleManager::~ModuleManager() {
    UnloadAllModules();
}

bool Moonlit::ModuleManager::TryGetModule(const std::filesystem::path& _path, ModuleInfo& _outModuleInfo) {
    HMODULE moduleHandle = LoadLibraryA(_path.string().c_str());
    if (moduleHandle == 0) {
        // DWORD error = GetLastError();
        // throw std::runtime_error("Failed to load module: " + _path.string() + " Error: " + std::to_string(error));
        LOG_WARNING("Couldn't load module " + _path.string());
        return false;
    }

    FARPROC registerFunction = GetProcAddress(moduleHandle, "GetMetadata");
    if (registerFunction == 0) {
        FreeLibrary(moduleHandle);
        LOG_WARNING("Module " + _path.string() + " does not contain metadata");
        return false;
    }

    auto getMetadataFunc = reinterpret_cast<ModuleMetadata(*)()>(registerFunction);
    ModuleMetadata metadata = getMetadataFunc();

    if (metadata.Name == nullptr) {
        // FreeLibrary(moduleHandle);
        // throw std::runtime_error("Module " + _path.string() + " has invalid metadata: Name is null.");
        LOG_WARNING("Module " + _path.string() + " has invalid metadata");
        return false;
    }

    registerFunction = GetProcAddress(moduleHandle, "GetRegistry");
    auto getBehaviourFunc = reinterpret_cast<ModuleBehaviourRegistry(*)()>(registerFunction);
    ModuleBehaviourRegistry moduleBehaviourRegistry = getBehaviourFunc();

    _outModuleInfo.Metadata = metadata;
    _outModuleInfo.BehaviourRegistry = moduleBehaviourRegistry;
    _outModuleInfo.ModuleHandle = moduleHandle;
    _outModuleInfo.ModulePath = _path.string();

    return true;
}

void Moonlit::ModuleManager::unloadModuleInternal(const std::string _name) {
    auto it = std::find_if(m_loadedModules.begin(), m_loadedModules.end(), [&_name](const ModuleInfo& plugin) {
        return plugin.Metadata.Name == _name;
    });

    if (it != m_loadedModules.end()) {
        FARPROC registerFunction = GetProcAddress(it->ModuleHandle, "GetRegistry");
        auto getBehaviourFunc = reinterpret_cast<ModuleBehaviourRegistry(*)()>(registerFunction);
        ModuleBehaviourRegistry moduleBehaviourRegistry = getBehaviourFunc();

        for (int i = 0; i < moduleBehaviourRegistry.EntryCount; i++)
        {
            auto& behaviourRegEntry = moduleBehaviourRegistry.Entries[i];
            BehaviourRegistry::UnregisterBehaviour(behaviourRegEntry.Name);
        }

        FreeLibrary(it->ModuleHandle);
        m_loadedModules.erase(it);
    } else {
        LOG_WARNING("Module with name " + _name + " not found, cannot unload.");
    }
}
