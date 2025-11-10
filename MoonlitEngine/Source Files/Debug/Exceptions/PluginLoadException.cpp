#include "Debug/Exceptions/PluginLoadException.h"

PluginLoadException::PluginLoadException(PluginLoadError _error, std::string _pluginName, HMODULE _pluginModule)
	: m_error(_error), m_pluginName(_pluginName), m_pluginModule(_pluginModule)
{
	m_whatMessage = "PluginLoadException: ";
	switch (m_error)
	{
	case PluginLoadError::FILE_NOT_FOUND:
		m_whatMessage += "Plugin file not found: ";
		break;
	case PluginLoadError::REGISTER_FUNCTION_MISSING:
		m_whatMessage += "Register function missing in plugin: ";
		break;
	case PluginLoadError::UNKNOWN_ERROR:
		m_whatMessage += "Unknown error while loading plugin: ";
		break;
	default:
		m_whatMessage += "Unrecognized error code in plugin: ";
		break;
	}

	m_whatMessage += m_pluginName;
}
