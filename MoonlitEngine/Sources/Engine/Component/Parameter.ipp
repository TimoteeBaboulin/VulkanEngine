#pragma once

#undef min
#undef max

#include "Engine/ResourceManagement/Serialization/adl_serializer.h"

template<typename PARAMETER_TYPE>
void Parameter<PARAMETER_TYPE>::Save(nlohmann::json& _json)
{
    ParameterBase::Save(_json);
    _json["value"] = m_value;
}

template<typename PARAMETER_TYPE>
void Parameter<PARAMETER_TYPE>::Load(nlohmann::json& _json)
{
    ParameterBase::Load(_json);
    m_value = _json["value"].get<PARAMETER_TYPE>();
}

