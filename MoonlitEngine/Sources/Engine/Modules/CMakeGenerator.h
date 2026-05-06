#pragma once
#include <string>
#include <vector>

#include "MoonlitExport.h"

class MOONLIT_API CMakeGenerator {
public:
    CMakeGenerator(std::string _projectName, std::string _targetName);

    CMakeGenerator& withProjectName(const std::string& _projectName);
    CMakeGenerator& withTargetName(const std::string& _targetName);

    CMakeGenerator& withSource(const std::string& _sourcePath);
    CMakeGenerator& withSources(const std::vector<std::string>& _sources);

    CMakeGenerator& withLinkLibrary(const std::string& _linkLibrary);
    CMakeGenerator& withLinkLibraries(const std::vector<std::string>& _linkLibraries);

    CMakeGenerator& withIncludePath(const std::string& _includePath);
    CMakeGenerator& withIncludePaths(const std::vector<std::string>& _includePaths);

    CMakeGenerator& withTargetProperty(const std::pair<std::string, std::string>& _targetProperty);
    CMakeGenerator& withTargetProperties(const std::vector<std::pair<std::string, std::string>>& _targetProperties);

    void writeTo(std::fstream& _stream);

private:
    std::string m_projectName;
    std::string m_targetName;

    std::vector<std::string> m_sourceFiles;
    std::vector<std::string> m_linkLibraries;
    std::vector<std::string> m_includePaths;
    std::vector<std::pair<std::string, std::string>> m_targetProperties;
};
