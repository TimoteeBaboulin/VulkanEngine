#ifndef MOONLIT_PARAMETERBASE_HPP
#define MOONLIT_PARAMETERBASE_HPP

#include "ParameterBase.h"
#include "Parameter.h"

template<typename PARAMETER_TYPE>
PARAMETER_TYPE ParameterBase::Value() const
{
    return static_cast<Parameter<PARAMETER_TYPE>*>(this)->Value<PARAMETER_TYPE>();
}

#endif //MOONLIT_PARAMETERBASE_HPP