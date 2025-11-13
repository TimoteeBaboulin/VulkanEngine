#pragma once

#include <qwidget.h>
#include <qlabel.h>

#include "Engine/Components/ObjectBehaviour.h"

class ParameterWidget : public QWidget
{
public:
	ParameterWidget(const ParameterRepositoryEntry& _entry);
	ParameterWidget(const ParameterWidget& _toCopy);

protected:
	void SetUI();

	const ParameterRepositoryEntry& m_entry;
	QLayout* m_layout;
};