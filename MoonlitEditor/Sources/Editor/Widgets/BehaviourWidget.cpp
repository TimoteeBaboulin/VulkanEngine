#include "BehaviourWidget.h"

#include "Debug/Logger.h"
#include "Engine/Component/BehaviourRegistry.h"

#include <qboxlayout.h>

BehaviourEditor::BehaviourEditor(Moonlit::ObjectBehaviour* _behaviour)
	: QFrame()
{
	m_behaviour = _behaviour;

	SetUI();
	ReadParameters();
}

BehaviourEditor::~BehaviourEditor()
{
	for (auto& widget : m_widgets)
	{
		delete widget;
	}
}

void BehaviourEditor::ReadParameters()
{
	while (m_layout->itemAt(1))
	{
		m_layout->removeItem(m_layout->itemAt(1));
	}

	for (auto& widget : m_widgets)
	{
		delete widget;
	}

	m_widgets.clear();

	if (!m_behaviour)
		return;

	std::vector<ParameterBase*> parameters = m_behaviour->GetParameters();

	for (auto parameter : parameters)
	{
		LOG_INFO("Type of parameter is " + std::string(typeid(parameter).name()));
	}
}

void BehaviourEditor::SetUI()
{
	setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
	setLineWidth(1);
	setMidLineWidth(0);

	m_layout = new QBoxLayout(QBoxLayout::TopToBottom);
	m_layout->setSpacing(6);
	m_layout->setContentsMargins(6, 6, 6, 6);
	setLayout(m_layout);

	m_name = new QLabel();
	std::string typeName = typeid(*m_behaviour).name();
	typeName = Moonlit::ClassNameFromTypeName(typeName.c_str());
	QString behaviourName = QString::fromStdString(typeName);
	m_name->setText(behaviourName);
	m_name->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	m_name->setAlignment(Qt::AlignLeft);
	m_layout->addWidget(m_name);

	QSizePolicy policy;
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}
