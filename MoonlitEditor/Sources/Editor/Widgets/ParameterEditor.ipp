#ifndef MOONLIT_PARAMETERWIDGET_IPP
#define MOONLIT_PARAMETERWIDGET_IPP

#include "ParameterEditor.h"

namespace Moonlit::Editor
{
    //TODO: Add a copy constructor to Event to pass the currently subscribed targets to the new event
    template<typename PARAMETER_TYPE>
    ParameterEditor<PARAMETER_TYPE>::ParameterEditor(ParameterEditor &&_toCopy) : ParameterEditorBase(_toCopy.m_parameterBase, _toCopy.m_behaviour), m_parameter(_toCopy.m_parameter), OnParameterChanged(this)
    {
        _toCopy.m_behaviour = nullptr;
    }

    template<typename PARAMETER_TYPE>
    ParameterEditor<PARAMETER_TYPE>& ParameterEditor<PARAMETER_TYPE>::operator=(ParameterEditor &&_toCopy)
    {
        m_parameter = _toCopy.m_parameter;
        m_parameterBase = _toCopy.m_parameterBase;
        m_behaviour = _toCopy.m_behaviour;

        _toCopy.m_behaviour = nullptr;
        _toCopy.m_parameterBase = nullptr;

        OnParameterChanged(this);

        return *this;
    }

    template<typename PARAMETER_TYPE>
    ParameterEditor<PARAMETER_TYPE>::~ParameterEditor()
    {

    }
}


template<typename PARAMETER_TYPE>
void ParameterEditorFactory::Register(
    std::function<ParameterEditorBase *(Parameter<PARAMETER_TYPE> *, Moonlit::ObjectBehaviour *)> _constructor)
{
    auto it = std::find(constructors.begin(), constructors.end(), _constructor);
    if (it != constructors.end())
    {
        LOG_WARNING("Trying to re-register for type " + std::string(typeid(PARAMETER_TYPE).name()));
        return;
    }

    constructors.insert(typeid(PARAMETER_TYPE), _constructor);
}

template<typename PARAMETER_TYPE>
bool ParameterEditorFactory::TryCreateParameterEditor(Parameter<PARAMETER_TYPE>* _param, Moonlit::ObjectBehaviour* _behaviour, ParameterEditorBase *&_outEditor)
{
    const type_info& paramTypeInfo = typeid(PARAMETER_TYPE);
    if (constructors.contains(paramTypeInfo))
    {
        _outEditor = constructors[paramTypeInfo](_param, _behaviour);
        return true;
    }

    _outEditor = nullptr;
    return false;
}

#endif //MOONLIT_PARAMETERWIDGET_IPP
