#include "ParameterWidget.h"

#include <qboxlayout.h>
#include <qlineedit.h>
#include <glm/glm.hpp>
#include <qregularexpressionvalidator>
#include <qregularexpression.h>

#include <sstream>
#include <iomanip>

#include "ParameterTypes/ParameterTypeRegistry.h"
#include "ParameterTypes/ParameterEditor.h"


ParameterWidget::ParameterWidget(const Moonlit::ParameterRepositoryEntry& _entry, Moonlit::ObjectBehaviour* _behaviour)
	: m_entry(_entry), QWidget(), m_behaviour(_behaviour)
{
	SetUI();
}

ParameterWidget::~ParameterWidget()
{
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
	m_layout->setContentsMargins(6, 7, 6, 6);
	m_layout->setAlignment(Qt::AlignLeft);

	ParamEditorFactory_t factory = ParameterTypeRegistry::Get().GetFactoryForType(m_entry.TypeName);
	if (factory != nullptr)
	{
		ParameterEditor* editor = factory(m_entry);
		editor->OnParameterChanged += [this](const Moonlit::ParameterRepositoryEntry& _entry) {
			LOG_INFO("Parameter has been changed on behaviour with type: " + std::string(typeid(*m_behaviour).name()) + ", parameter name: " + std::string(_entry.Name));
			m_behaviour->ParameterChanged(_entry);
		};
		editor->AddToLayout(m_layout);
	}

	setLayout(m_layout);
}
