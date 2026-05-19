#pragma once

#include <filesystem>

namespace Moonlit::Editor
{
    class ProjectPaths
    {
    public:
        void Init(const std::filesystem::path& _projectRoot);
        void SetProjectRoot(const std::filesystem::path& _projectRoot);

        std::filesystem::path GetProjectRoot() const;
        std::filesystem::path GetPath(const std::filesystem::path& _relative) const;
        std::filesystem::path GetResourcesPath() const;
        std::filesystem::path GetModulesPath() const;
        std::filesystem::path GetTempModulePath() const;

    private:
        std::filesystem::path m_projectRoot;
    };
}
