#ifndef MOONLIT_PARAMETER_IPP
#define MOONLIT_PARAMETER_IPP

template<typename PARAMETER_TYPE>
void Parameter<PARAMETER_TYPE>::Save(std::ofstream &_stream)
{
    ParameterBase::Save(_stream);
    char* buffer = new char[sizeof(PARAMETER_TYPE)];
    std::memcpy(buffer, &m_value, sizeof(PARAMETER_TYPE));
    _stream << buffer << std::endl;
    delete[] buffer;
}

template<typename PARAMETER_TYPE>
void Parameter<PARAMETER_TYPE>::Load(std::ifstream &_stream)
{
    ParameterBase::Load(_stream);

    size_t size = sizeof(PARAMETER_TYPE);
    char* buffer = new char[size];
    _stream.get(buffer, static_cast<std::streamsize>(size));
    std::memcpy(&m_value, buffer, size);
    _stream.read(buffer, 1); // Take care of the endline
    delete[] buffer;
}

#endif //MOONLIT_PARAMETER_IPP
