#include "Editor/Windows/Widgets/ParameterWidget.h"

#include <qboxlayout.h>

ParameterWidget::ParameterWidget(const ParameterRepositoryEntry& _entry)
	: m_entry(_entry), QWidget()
{
	SetUI();
}

ParameterWidget::ParameterWidget(const ParameterWidget& _toCopy)
	: m_entry(_toCopy.m_entry), QWidget()
{
	SetUI();
}

void ParameterWidget::SetUI()
{
	m_layout = new QBoxLayout(QBoxLayout::LeftToRight);
	QLabel* name = new QLabel(QString::fromStdString(m_entry.Name));
	name->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	name->setAlignment(Qt::AlignLeft);
	m_layout->addWidget(name);
	m_layout->setSpacing(6);
	m_layout->setContentsMargins(6, 6, 6, 6);
	m_layout->setAlignment(Qt::AlignLeft);

	setLayout(m_layout);
}
