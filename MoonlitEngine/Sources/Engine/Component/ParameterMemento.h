#pragma once

#include "Parameter.h"

template <typename PARAMETER_TYPE>
class ParameterMemento {
public:
    ParameterMemento(Parameter<PARAMETER_TYPE>& _parameter);
    virtual ~ParameterMemento() = default;

    virtual void SaveParameter();
    virtual void LoadParameter();
protected:
    Parameter<PARAMETER_TYPE>& m_parameter;
    PARAMETER_TYPE m_value;
};

template<typename PARAMETER_TYPE>
ParameterMemento<PARAMETER_TYPE>::ParameterMemento(Parameter<PARAMETER_TYPE> &_parameter)
{
    m_parameter = _parameter;
    m_value = m_parameter.Value();
}

template<typename PARAMETER_TYPE>
void ParameterMemento<PARAMETER_TYPE>::SaveParameter()
{
    m_value = m_parameter.Value();
}

template<typename PARAMETER_TYPE>
void ParameterMemento<PARAMETER_TYPE>::LoadParameter()
{
    m_parameter.SetValue(m_value);
}

