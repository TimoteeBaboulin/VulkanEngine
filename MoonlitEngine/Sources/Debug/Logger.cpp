#include "Logger.h"

#include <Windows.h>
#include <iostream>

Moonlit::Debug::LoggerBase* Moonlit::Debug::Logger::m_loggerInstance = nullptr;

void Moonlit::Debug::Logger::LogMessage(const DebugMessage& debugMessage)
{
	if (m_loggerInstance)
	{
		m_loggerInstance->LogMessage(debugMessage);
		return;
	}
	
#ifdef _DEBUG
	HANDLE _consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	switch (debugMessage.Type)
	{
		case DebugMessage::MessageType::Info:
		{
			SetConsoleTextAttribute(_consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			std::cout << "[INFO]\t\t";
			break;
		}
		case DebugMessage::MessageType::Warning:
		{
			SetConsoleTextAttribute(_consoleHandle, FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
			std::cout << "[WARNING]\t\t";
			break;
		}
		case DebugMessage::MessageType::Error:
		{
			SetConsoleTextAttribute(_consoleHandle, FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_INTENSITY);
			std::cout << "[ERROR]\t\t";
			break;
		}
	}

	SetConsoleTextAttribute(_consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	std::cout << debugMessage.Message << std::endl;
#endif
}

void Moonlit::Debug::Logger::LogInfo(const char* message)
{
	DebugMessage debugMessage;
	debugMessage.Message = message;
	debugMessage.Type = DebugMessage::MessageType::Info;

	Logger::LogMessage(debugMessage);
}

void Moonlit::Debug::Logger::LogInfo(const std::string& _message)
{
	DebugMessage debugMessage;
	debugMessage.Message = _message.c_str();
	debugMessage.Type = DebugMessage::MessageType::Info;
	Logger::LogMessage(debugMessage);
}

void Moonlit::Debug::Logger::LogWarning(const char* message)
{
	DebugMessage debugMessage;
	debugMessage.Message = message;
	debugMessage.Type = DebugMessage::MessageType::Warning;

	Logger::LogMessage(debugMessage);
}

void Moonlit::Debug::Logger::LogWarning(const std::string& _message)
{
	DebugMessage debugMessage;
	debugMessage.Message = _message.c_str();
	debugMessage.Type = DebugMessage::MessageType::Warning;
	Logger::LogMessage(debugMessage);
}

void Moonlit::Debug::Logger::LogError(const char* message)
{
	DebugMessage debugMessage;
	debugMessage.Message = message;
	debugMessage.Type = DebugMessage::MessageType::Error;

	Logger::LogMessage(debugMessage);
}

void Moonlit::Debug::Logger::LogError(const std::string& _message)
{
	DebugMessage debugMessage;
	debugMessage.Message = _message.c_str();
	debugMessage.Type = DebugMessage::MessageType::Error;
	Logger::LogMessage(debugMessage);
}
