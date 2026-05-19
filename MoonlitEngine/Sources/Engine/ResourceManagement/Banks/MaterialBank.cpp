#include "MaterialBank.h"

#include <filesystem>

#include "Debug/Logger.h"

bool Moonlit::ResourceManagement::MaterialBank::TryLoad(const std::string& _filepath)
{
    namespace fs = std::filesystem;

    fs::path path(_filepath);
    if (!fs::is_regular_file(path))
    {
        LOG_ERROR("MaterialBank::TryLoad - File not found: " + _filepath);
        return false;
    }

    if (path.extension().string() != ".slang")
        return false;

    std::string name = path.stem().string();

    if (Exist(name))
    {
        LOG_WARNING("MaterialBank::TryLoad - Material already loaded: " + name);
        return false;
    }

    try
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_resources.emplace(name, _filepath);
    }
    catch (const std::exception& e)
    {
        LOG_ERROR("MaterialBank::TryLoad - Failed to construct material at " + _filepath + ": " + e.what());
        return false;
    }

    return true;
}
