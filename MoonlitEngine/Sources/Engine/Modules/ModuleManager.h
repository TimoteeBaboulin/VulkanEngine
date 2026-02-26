#ifndef MOONLIT_PLUGINMANAGER_H
#define MOONLIT_PLUGINMANAGER_H

#include "../../../Sources/MoonlitExport.h"
#include "Module.h"

#include <vector>
#include <string>
#include <vector>

constexpr int DefaultModuleReserveSize = 10;

namespace Moonlit {
    class MOONLIT_API ModuleManager {
    public:
        static ModuleManager& Get() {
            static ModuleManager instance;
            return instance;
        }

        void LoadModulesFromDirectory(std::string _directory);
        void UnloadAllModules();
        void UnloadModule(std::string _name);
        void LoadModule(std::filesystem::path _path);

        const std::vector<ModuleInfo>& GetPlugins() const { return m_loadedModules; }

    private:
        ModuleManager();
        ~ModuleManager();

        std::vector<ModuleInfo> m_loadedModules;
    };
}

#endif //MOONLIT_PLUGINMANAGER_H