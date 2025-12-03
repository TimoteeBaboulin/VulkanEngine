#pragma once

#include <qwidget.h>
#include <qlabel.h>

#include "Engine/Components/ObjectBehaviour.h"
#include "ParameterWidget.h"

class BehaviourEditor : public QFrame
{
public:
	BehaviourEditor(ObjectBehaviour* _behaviour);
	~BehaviourEditor();

private:
	void ReadParameters();
	void SetUI();

	ObjectBehaviour* m_behaviour = nullptr;
	QLayout* m_layout = nullptr;
	QLabel* m_name = nullptr;
	std::vector<ParameterWidget*> m_widgets;
};