#pragma once
#include <string>
#include <unordered_map>

namespace Moonlit::Editor
{
    class CommandLineParser
    {
    public:
        static void Parse(int _argc, char** _argv);
        static bool TryGetValue(const std::string& _flag, std::string& _outValue);
        static bool HasFlag(const std::string& _flag);

    private:
        static CommandLineParser& Get();
        void ParseInternal(int _argc, char** _argv);
        bool TryGetValueInternal(const std::string& _flag, std::string& _outValue);

        std::unordered_map<std::string, std::string> m_args;
        std::unordered_map<std::string, bool> m_flags;
    };
}
