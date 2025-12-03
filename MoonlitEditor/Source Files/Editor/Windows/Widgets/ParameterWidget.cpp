#include "Editor/Windows/Widgets/ParameterWidget.h"

#include <qboxlayout.h>
#include <qlineedit.h>
#include <glm/glm.hpp>
#include <qregularexpressionvalidator>
#include <qregularexpression.h>

#include <sstream>
#include <iomanip>

#include "Editor/Windows/Widgets/ParameterTypes/ParameterTypeRegistry.h"
#include "Editor/Windows/Widgets/ParameterTypes/ParameterEditor.h"


ParameterWidget::ParameterWidget(const ParameterRepositoryEntry& _entry, ObjectBehaviour* _behaviour)
	: m_entry(_entry), QWidget(), m_behaviour(_behaviour)
{
	SetUI();
}

ParameterWidget::ParameterWidget(const ParameterWidget& _toCopy)
	: m_entry(_toCopy.m_entry), QWidget()
{
	SetUI();
}

ParameterWidget::~ParameterWidget()
{
	for (ParameterEditor* editor : m_parameterEditors)
	{
		delete editor;
	}

	delete m_name;
	delete m_layout;
}

void ParameterWidget::SetUI()
{
	m_layout = new QBoxLayout(QBoxLayout::LeftToRight);
	m_name = new QLabel(QString::fromStdString(m_entry.Name));
	m_name->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	m_name->setAlignment(Qt::AlignLeft);
	m_layout->addWidget(m_name);
	m_layout->setSpacing(6);
	m_layout->setContentsMargins(6, 6, 6, 6);
	m_layout->setAlignment(Qt::AlignLeft);

	ParamEditorFactory_t factory = ParameterTypeRegistry::Get().GetFactoryForType(m_entry.TypeName);
	if (factory != nullptr)
	{
		ParameterEditor* editor = factory(m_entry);
		editor->OnParameterChanged += std::bind(&ObjectBehaviour::ParameterChanged, m_behaviour);
		editor->AddToLayout(m_layout);
	}

	setLayout(m_layout);
}
