#ifndef MOONLIT_PARAMETER_H
#define MOONLIT_PARAMETER_H

#include <string>
#include "ParameterBase.h"

template <typename PARAMETER_TYPE>
class Parameter : public ParameterBase{
public:
    Parameter(std::string _name) : ParameterBase(_name) {};
    template <typename... ARGS>
    Parameter(std::string _name, ARGS... _args) : ParameterBase(_name), m_value(_args...) {};
    Parameter(std::string _name, PARAMETER_TYPE _value) : ParameterBase(_name), m_value(_value) {};
    Parameter(Parameter& _copy) : ParameterBase(_copy.m_name), m_value(_copy.m_value) {};
    ~Parameter() = default;

    Parameter& operator=(PARAMETER_TYPE _value)
    {
        m_value = _value;
        return *this;
    }
    PARAMETER_TYPE operator*()
    {
        return m_value;
    }

    PARAMETER_TYPE Value() const {return m_value;};
    void SetValue(PARAMETER_TYPE value) {m_value = value;};

protected:
    PARAMETER_TYPE m_value;
};

#endif //MOONLIT_PARAMETER_H
