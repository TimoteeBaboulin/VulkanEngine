#pragma once

#include <qwidget.h>
#include "Engine/Components/ObjectBehaviour.h"
#include "ParameterWidget.h"

class BehaviourWidget : public QFrame
{
public:
	BehaviourWidget(ObjectBehaviour* _behaviour);

private:
	void ReadParameters();
	void SetUI();

	ObjectBehaviour* m_behaviour = nullptr;
	QLayout* m_layout = nullptr;
	std::vector<ParameterWidget*> m_widgets;
};