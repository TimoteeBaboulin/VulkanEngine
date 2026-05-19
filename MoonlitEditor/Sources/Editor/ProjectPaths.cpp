#include "ProjectPaths.h"

namespace Moonlit::Editor
{
    void ProjectPaths::Init(const std::filesystem::path& _projectRoot)
    {
        m_projectRoot = _projectRoot;
    }

    void ProjectPaths::SetProjectRoot(const std::filesystem::path& _projectRoot)
    {
        m_projectRoot = _projectRoot;
    }

    std::filesystem::path ProjectPaths::GetProjectRoot() const
    {
        return m_projectRoot;
    }

    std::filesystem::path ProjectPaths::GetPath(const std::filesystem::path& _relative) const
    {
        return m_projectRoot / _relative;
    }

    std::filesystem::path ProjectPaths::GetResourcesPath() const
    {
        return m_projectRoot / "Resources";
    }

    std::filesystem::path ProjectPaths::GetModulesPath() const
    {
        return m_projectRoot / "Modules";
    }

    std::filesystem::path ProjectPaths::GetTempModulePath() const
    {
        return m_projectRoot / "Temp";
    }
}
