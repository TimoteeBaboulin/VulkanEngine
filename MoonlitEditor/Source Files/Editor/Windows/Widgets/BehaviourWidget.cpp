#include "Editor/Windows/Widgets/BehaviourWidget.h"

#include "Debug/Logger.h"

#include <qboxlayout.h>

BehaviourWidget::BehaviourWidget(ObjectBehaviour* _behaviour)
	: QFrame()
{
	SetUI();
	m_behaviour = _behaviour;
	ReadParameters();
}

void BehaviourWidget::ReadParameters()
{
	while (m_layout->itemAt(0))
	{
		m_layout->removeItem(m_layout->itemAt(0));
	}

	for (auto& widget : m_widgets)
	{
		delete widget;
	}

	m_widgets.clear();

	if (!m_behaviour)
		return;

	std::vector<ParameterRepositoryEntry> parameters = m_behaviour->GetParameterEntries();

	for (auto& parameter : parameters)
	{
		std::string paramMessage = "Reading parameter ";
		paramMessage += parameter.Name;
		paramMessage += " of type ";
		paramMessage += parameter.TypeName;
		LOG_INFO(paramMessage.c_str());
		m_widgets.push_back(new ParameterWidget(parameter));
		m_layout->addWidget(m_widgets.back());
	}
}

void BehaviourWidget::SetUI()
{
	setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
	setLineWidth(1);
	setMidLineWidth(0);

	m_layout = new QBoxLayout(QBoxLayout::TopToBottom);
	m_layout->setSpacing(6);
	m_layout->setContentsMargins(6, 6, 6, 6);
	setLayout(m_layout);

	QSizePolicy policy;
	//policy.setVerticalPolicy(QSizePolicy)
	//setSizePolicy()
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}
