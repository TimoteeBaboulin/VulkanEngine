#ifndef MOONLIT_PARAMETERWIDGET_H
#define MOONLIT_PARAMETERWIDGET_H

#include "ParameterEditorBase.h"
#include "Engine/Component/ObjectBehaviour.h"
#include "Engine/Component/Parameter.h"

namespace Moonlit::Editor
{
template <typename PARAMETER_TYPE>
class ParameterEditor
    : ParameterEditorBase {
        using Parameter = Parameter<PARAMETER_TYPE>;
    public:
        // Rule of 5: Here we allow moving but not copying
        ParameterEditor(Parameter* _entry, ObjectBehaviour* _behaviour) : ParameterEditorBase(*_entry, _behaviour), m_parameter(_entry) {};
        ParameterEditor(const ParameterEditor& _toCopy) = delete;
        ParameterEditor(ParameterEditor&& _toCopy);
        ParameterEditor& operator=(const ParameterEditor& _toCopy) = delete;
        ParameterEditor& operator=(ParameterEditor&& _toCopy);
        ~ParameterEditor();

    private:
        Parameter& m_parameter;
    };
}

class ParameterEditorFactory {
    using ParameterEditorFactoryConstructor = std::function<ParameterEditorBase*(ParameterBase*, Moonlit::ObjectBehaviour*)>;
public:
    template<typename PARAMETER_TYPE>
    void Register(std::function<ParameterEditorBase*(Parameter<PARAMETER_TYPE>*, Moonlit::ObjectBehaviour*)> _constructor);

    template<typename PARAMETER_TYPE>
    bool TryCreateParameterEditor(Parameter<PARAMETER_TYPE>* _param, Moonlit::ObjectBehaviour* _behaviour, ParameterEditorBase*& _outEditor);
private:
    std::map<std::type_info, ParameterEditorFactoryConstructor> constructors;
};


#endif //MOONLIT_PARAMETERWIDGET_H
