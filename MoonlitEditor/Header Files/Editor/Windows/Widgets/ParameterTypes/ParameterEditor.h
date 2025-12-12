#pragma once

#include "Engine/Components/ObjectBehaviour.h"
#include "Engine/Events/Event.h"

#include <qwidget.h>

//Helper functions

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


class ParameterEditor : public QWidget
{
public:
	Event<void, const ParameterRepositoryEntry&> OnParameterChanged;

	// Default constructor
	ParameterEditor(const ParameterRepositoryEntry& _entry, QWidget* _parent = nullptr)
		: QWidget(_parent), m_entry(_entry)
	{
	}

	virtual void AddToLayout(QLayout* _layout) = 0;
	virtual void RemoveFromLayout(QLayout* _layout) = 0;

protected:
	ParameterRepositoryEntry m_entry;
};