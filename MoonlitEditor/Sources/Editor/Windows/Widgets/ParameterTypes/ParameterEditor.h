#pragma once

#include "Engine/Component/ObjectBehaviour.h"
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
	Moonlit::Events::Event<void, const Moonlit::ParameterRepositoryEntry&> OnParameterChanged;

	// Default constructor
	ParameterEditor(const Moonlit::ParameterRepositoryEntry& _entry, QWidget* _parent = nullptr)
		: QWidget(_parent), m_entry(_entry)
	{
	}

	virtual void AddToLayout(QLayout* _layout) = 0;
	virtual void RemoveFromLayout(QLayout* _layout) = 0;

protected:
	Moonlit::ParameterRepositoryEntry m_entry;
};