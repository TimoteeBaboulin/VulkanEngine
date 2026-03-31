#ifndef MOONLIT_PARAMETERBASE_H
#define MOONLIT_PARAMETERBASE_H

#include "Engine/ResourceManagement/Helpers/FileHelper.h"

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

    virtual void Load(std::ifstream &_stream) {};
    virtual void Save(std::ofstream& _stream)
    {
        Moonlit::FileHelper::WriteStringBinary(_stream, m_name);
    };

protected:
    std::string m_name;
};

#endif //MOONLIT_PARAMETERBASE_H