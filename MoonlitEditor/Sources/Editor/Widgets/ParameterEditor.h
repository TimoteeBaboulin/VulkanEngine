#ifndef MOONLIT_PARAMETERWIDGET_H
#define MOONLIT_PARAMETERWIDGET_H

#include "ParameterEditorBase.h"
#include "Engine/Component/ObjectBehaviour.h"
#include "Engine/Component/Parameter.h"
#include "Engine/Events/Event.h"

class ParameterEditorHelper
{
public:
    static std::string GetPrettiestFloat(float _value)
    {
        std::ostringstream oss;
        oss << std::setprecision(8) << std::noshowpoint << _value;
        return oss.str();
    }
};

namespace Moonlit::Editor
{
template <typename PARAMETER_TYPE>
class ParameterEditor
    : public ParameterEditorBase {
    protected:
        using Parameter = Parameter<PARAMETER_TYPE>;
        using EventType = Events::Event<void, ObjectBehaviour*, PARAMETER_TYPE>;
    public:
        // Rule of 5: Here we allow moving but not copying
        ParameterEditor(Parameter* _entry, ObjectBehaviour* _behaviour, QWidget* _parent = nullptr) : ParameterEditorBase(_entry, _behaviour, _parent), m_parameter(*_entry), OnParameterChanged(this) {};
        ParameterEditor(const ParameterEditor& _toCopy) = delete;
        ParameterEditor(ParameterEditor&& _toCopy);
        ParameterEditor& operator=(const ParameterEditor& _toCopy) = delete;
        ParameterEditor& operator=(ParameterEditor&& _toCopy);
        ~ParameterEditor();

        EventType OnParameterChanged;
    protected:
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

#include "ParameterEditor.ipp"

#endif //MOONLIT_PARAMETERWIDGET_H
