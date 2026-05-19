#include "CommandLineParser.h"

namespace Moonlit::Editor
{
    void CommandLineParser::Parse(int _argc, char** _argv)
    {
        Get().ParseInternal(_argc, _argv);
    }

    bool CommandLineParser::TryGetValue(const std::string& _flag, std::string& _outValue)
    {
        return Get().TryGetValueInternal(_flag, _outValue);
    }

    bool CommandLineParser::HasFlag(const std::string& _flag)
    {
        auto& flags = Get().m_flags;
        return flags.find(_flag) != flags.end();
    }

    CommandLineParser& CommandLineParser::Get()
    {
        static CommandLineParser instance;
        return instance;
    }

    void CommandLineParser::ParseInternal(int _argc, char** _argv)
    {
        for (int i = 1; i < _argc; ++i)
        {
            std::string arg = _argv[i];
            if (!arg.starts_with("--"))
                continue;

            std::string key = arg.substr(2);
            if (i + 1 < _argc && !std::string(_argv[i + 1]).starts_with("--"))
            {
                m_args[key] = _argv[i + 1];
                ++i;
            }
            else
            {
                m_flags[key] = true;
            }
        }
    }

    bool CommandLineParser::TryGetValueInternal(const std::string& _flag, std::string& _outValue)
    {
        auto it = m_args.find(_flag);
        if (it == m_args.end())
            return false;

        _outValue = it->second;
        return true;
    }
}
