#pragma once

#include <exception>
#include <string>
#include <Windows.h>

namespace Moonlit::Debug::Exceptions
{

	enum struct PluginLoadError
	{
		FILE_NOT_FOUND,
		REGISTER_FUNCTION_MISSING,
		UNKNOWN_ERROR
	};

	class PluginLoadException : public std::exception
	{
	public:
		PluginLoadException(PluginLoadError _error, std::string _pluginName, HMODULE _pluginModule = nullptr);
		PluginLoadError Error() const { return m_error; }
		std::string PluginName() const { return m_pluginName; }
		HMODULE PluginModule() const { return m_pluginModule; }
		const char* what() const override { return m_whatMessage.c_str(); }

	private:
		PluginLoadError m_error;
		std::string m_pluginName;
		HMODULE m_pluginModule;

		std::string m_whatMessage;
	};
}