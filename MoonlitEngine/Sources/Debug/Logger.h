#pragma once

#define LOG_INFO(message) Moonlit::Debug::Logger::LogInfo(message)
#define LOG_WARNING(message) Moonlit::Debug::Logger::LogWarning(message)
#define LOG_ERROR(message) Moonlit::Debug::Logger::LogError(message)

#define TEXTLOG(string) (string).c_str()

#include "../MoonlitExport.h"

#include <string>

namespace Moonlit::Debug
{
	struct MOONLIT_API DebugMessage
	{
		const char* Message;
		enum class MessageType
		{
			Info,
			Warning,
			Error
		} Type;
	};

	class MOONLIT_API LoggerBase
	{
	public:
		virtual void LogMessage(const DebugMessage& debugMessage) = 0;
	};

	//TODO: Implement string based logging to avoid creating string every time
	class MOONLIT_API Logger
	{
	public:
		static void LogMessage(const DebugMessage& debugMessage);

		static void LogInfo(const char* _message);
		static void LogInfo(const std::string& _message);
		static void LogWarning(const char* _message);
		static void LogWarning(const std::string& _message);
		static void LogError(const char* _message);
		static void LogError(const std::string& _message);

		static void SetLoggerInstance(LoggerBase* loggerInstance) { m_loggerInstance = loggerInstance; }

	private:
		static LoggerBase* m_loggerInstance;
	};
}