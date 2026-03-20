#include "ParameterEditorBase.h"

#include <qboxlayout.h>
#include <qlineedit.h>
#include <glm/glm.hpp>
#include <qregularexpressionvalidator>
#include <qregularexpression.h>

#include <sstream>
#include <iomanip>

#include "ParameterTypes/ParameterTypeRegistry.h"
#include "ParameterTypes/ParameterEditor.h"


ParameterEditorBase::ParameterEditorBase(const ParameterBase* _parameter, Moonlit::ObjectBehaviour* _behaviour)
	: QWidget(), m_parameterBase(_parameter), m_behaviour(_behaviour)
{
	SetUI();
}

ParameterEditorBase::~ParameterEditorBase() noexcept(true)
{
	delete m_name;
	delete m_layout;
}

void ParameterEditorBase::SetUI()
{
	m_layout = new QBoxLayout(QBoxLayout::LeftToRight);
	m_name = new QLabel(QString::fromStdString(m_parameterBase->Name()));
	m_name->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	m_name->setAlignment(Qt::AlignLeft);
	m_layout->addWidget(m_name);
	m_layout->setSpacing(6);
	m_layout->setContentsMargins(6, 7, 6, 6);
	m_layout->setAlignment(Qt::AlignLeft);

	setLayout(m_layout);

	SetParameterUI();
}
