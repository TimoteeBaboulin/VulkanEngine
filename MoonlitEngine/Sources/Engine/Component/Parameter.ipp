#ifndef MOONLIT_PARAMETER_IPP
#define MOONLIT_PARAMETER_IPP


template<typename PARAMETER_TYPE>
void Parameter<PARAMETER_TYPE>::Save(nlohmann::json& _json)
{
    ParameterBase::Save(_json);
    _json["value"] = m_value;
    // char* buffer = new char[sizeof(PARAMETER_TYPE)];
    // std::memcpy(buffer, &m_value, sizeof(PARAMETER_TYPE));
    // _stream << buffer << std::endl;
    // delete[] buffer;
}

template<typename PARAMETER_TYPE>
void Parameter<PARAMETER_TYPE>::Load(nlohmann::json& _json)
{
    ParameterBase::Load(_json);
    m_value = _json["value"].get<PARAMETER_TYPE>();

    // size_t size = sizeof(PARAMETER_TYPE);
    // char* buffer = new char[size];
    // _json.get(buffer, static_cast<std::streamsize>(size));
    // std::memcpy(&m_value, buffer, size);
    // _json.read(buffer, 1); // Take care of the endline
    // delete[] buffer;
}

#endif //MOONLIT_PARAMETER_IPP
