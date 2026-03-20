#ifndef MOONLIT_PARAMETERBASE_H
#define MOONLIT_PARAMETERBASE_H

class ParameterBase {
public:
    ParameterBase(std::string _name)
        : m_name(_name) {}

    template <typename PARAMETER_TYPE>
    PARAMETER_TYPE Value() const;

    std::string Name() const;

protected:
    std::string m_name;
};

#endif //MOONLIT_PARAMETERBASE_H