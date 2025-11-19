#pragma once

#include <qwidget.h>
#include <qlabel.h>

#include "Engine/Components/ObjectBehaviour.h"

class ParameterWidget : public QWidget
{
public:
	ParameterWidget(const ParameterRepositoryEntry& _entry, ObjectBehaviour* _behaviour);
	ParameterWidget(const ParameterWidget& _toCopy);

	~ParameterWidget() noexcept(true) override;

protected:
	void SetUI();

	const ParameterRepositoryEntry& m_entry;
	ObjectBehaviour* m_behaviour;
	QLayout* m_layout;

	std::vector<class ParameterEditor*> m_parameterEditors;
};