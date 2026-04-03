#pragma once

#include <nlohmann/json.hpp>

class ParameterBase {
public:
    ParameterBase(std::string _name)
        : m_name(_name) {}
    virtual ~ParameterBase() {}

    template <typename PARAMETER_TYPE>
    PARAMETER_TYPE Value() const;

    std::string Name() const
    {
        return m_name;
    }

    virtual void Load(nlohmann::json& _stream)
    {
        m_name = _stream["name"].get<std::string>();
    };
    virtual void Save(nlohmann::json& _stream)
    {
        _stream["name"] = m_name;
    };

protected:
    std::string m_name;
};