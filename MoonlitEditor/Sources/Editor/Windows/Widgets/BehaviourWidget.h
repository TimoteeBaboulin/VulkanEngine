#pragma once

#include <qwidget.h>
#include <qlabel.h>

#include "../../../../../MoonlitEngine/Sources/Engine/Component/ObjectBehaviour.h"
#include "ParameterWidget.h"

class BehaviourEditor : public QFrame
{
public:
	BehaviourEditor(Moonlit::ObjectBehaviour* _behaviour);
	~BehaviourEditor();

private:
	void ReadParameters();
	void SetUI();

	Moonlit::ObjectBehaviour* m_behaviour = nullptr;
	QLayout* m_layout = nullptr;
	QLabel* m_name = nullptr;
	std::vector<ParameterWidget*> m_widgets;
};