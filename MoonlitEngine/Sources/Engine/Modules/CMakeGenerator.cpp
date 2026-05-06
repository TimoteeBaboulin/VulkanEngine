#include "CMakeGenerator.h"

#include <fstream>

CMakeGenerator::CMakeGenerator(std::string _projectName, std::string _targetName)
    : m_projectName(_projectName), m_targetName(_targetName)
{

}

CMakeGenerator& CMakeGenerator::withProjectName(const std::string& _projectName)
{
    m_projectName = _projectName;
    return *this;
}

CMakeGenerator& CMakeGenerator::withTargetName(const std::string& _targetName)
{
    m_targetName = _targetName;
    return *this;
}

CMakeGenerator& CMakeGenerator::withSource(const std::string& _sourcePath)
{
    m_sourceFiles.push_back(_sourcePath);
    return *this;
}

CMakeGenerator& CMakeGenerator::withSources(const std::vector<std::string>& _sources)
{
    for (auto& source : _sources)
    {
        m_sourceFiles.push_back(source);
    }
    return *this;
}

CMakeGenerator& CMakeGenerator::withLinkLibrary(const std::string& _linkLibrary)
{
    m_linkLibraries.push_back(_linkLibrary);
    return *this;
}

CMakeGenerator& CMakeGenerator::withLinkLibraries(const std::vector<std::string>& _linkLibraries)
{
    for (auto& linkLibraries : _linkLibraries)
    {
        m_linkLibraries.push_back(linkLibraries);
    }
    return *this;
}

CMakeGenerator& CMakeGenerator::withIncludePath(const std::string& _includePath)
{
    m_includePaths.push_back(_includePath);
    return *this;
}

CMakeGenerator& CMakeGenerator::withIncludePaths(const std::vector<std::string>& _includePaths)
{
    for (auto& includePath : _includePaths)
    {
        m_includePaths.push_back(includePath);
    }
    return *this;
}

CMakeGenerator& CMakeGenerator::withTargetProperty(const std::pair<std::string, std::string>& _targetProperty)
{
    m_targetProperties.push_back(_targetProperty);
    return *this;
}

CMakeGenerator& CMakeGenerator::withTargetProperties(
    const std::vector<std::pair<std::string, std::string>>& _targetProperties)
{
    for (auto& targetProperty : _targetProperties)
    {
        m_targetProperties.push_back(targetProperty);
    }
    return *this;
}

void appendStrings(std::string& _dist, const std::vector<std::string>& _strings)
{
    for (auto& string : _strings)
    {
        _dist += " \"" + string + "\"";
    }
}

void CMakeGenerator::writeTo(std::fstream &_stream)
{
    std::string cMakeStr = "CMAKE_MINIMUM_REQUIRED(VERSION 3.1...4.2.3)";
    cMakeStr += "\nPROJECT(" + m_projectName + ")";

    cMakeStr += "\nADD_LIBRARY(" + m_targetName + " SHARED";
    appendStrings(cMakeStr, m_sourceFiles);
    cMakeStr += ")";

    cMakeStr += "\nTARGET_INCLUDE_DIRECTORIES(" + m_targetName + " PRIVATE";
    appendStrings(cMakeStr, m_includePaths);
    cMakeStr += ")";

    cMakeStr += "\nTARGET_LINK_LIBRARIES(" + m_targetName + " PRIVATE";
    appendStrings(cMakeStr, m_linkLibraries);
    cMakeStr += ")";

    if (!m_targetProperties.empty())
    {
        cMakeStr += "SET_TARGET_PROPERTIES(" + m_targetName + " PROPERTIES";
        for (auto& property : m_targetProperties)
        {
            cMakeStr += " " + property.first + " " + property.second;
        }
        cMakeStr += ")";
    }

    _stream.write(cMakeStr.c_str(), cMakeStr.length());
}
