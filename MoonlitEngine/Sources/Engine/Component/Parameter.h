#ifndef MOONLIT_PARAMETER_H
#define MOONLIT_PARAMETER_H

#include <string>
#include "ParameterBase.h"

template <typename PARAMETER_TYPE>
class Parameter : ParameterBase{
public:
    Parameter(std::string _name) : ParameterBase(_name) {};
    Parameter(std::string _name, PARAMETER_TYPE _value) : ParameterBase(_name), m_value(_value) {};
    ~Parameter() = default;

    PARAMETER_TYPE Value() const {return m_value;};
    void SetValue(PARAMETER_TYPE value) {m_value = value;};

protected:
    PARAMETER_TYPE m_value;
};

#endif //MOONLIT_PARAMETER_H
