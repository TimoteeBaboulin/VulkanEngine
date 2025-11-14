#include "Editor/Windows/Widgets/ParameterWidget.h"

#include <qboxlayout.h>
#include <qlineedit.h>
#include <glm/glm.hpp>
#include <qregularexpressionvalidator>
#include <qregularexpression.h>

#include <sstream>
#include <iomanip>

std::string GetPrettyFloat(float _value)
{
	std::ostringstream oss;
	oss << std::setprecision(8) << std::noshowpoint << _value;
	return oss.str();
}

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

	if (!strcmp(m_entry.TypeName, typeid(glm::vec3).name()))
	{
		glm::vec3* data = (glm::vec3*)(m_entry.Data);
		QRegularExpression* regex = new QRegularExpression("^[0-9,\.]*$");
		QRegularExpressionValidator* validator = new QRegularExpressionValidator();
		validator->setRegularExpression(*regex);

		//delete the regex now to avoid memory leaks
		delete regex;

		QLineEdit* xEdit = new QLineEdit(this);
		xEdit->setValidator(validator);
		xEdit->setText(QString::fromStdString(GetPrettyFloat(data->x)));
		connect(xEdit, &QLineEdit::editingFinished, this, [data, xEdit, this]()
			{
				std::istringstream stream(xEdit->text().toStdString());
				stream >> data->x;
				m_behaviour->ParameterChanged();
			});

		QLineEdit* yEdit = new QLineEdit(this);
		yEdit->setValidator(validator);
		yEdit->setText(QString::fromStdString(GetPrettyFloat(data->y)));
		
		QLineEdit* zEdit = new QLineEdit(this);
		zEdit->setValidator(validator);
		zEdit->setText(QString::fromStdString(GetPrettyFloat(data->z)));

		m_layout->addWidget(xEdit);
		m_layout->addWidget(yEdit);
		m_layout->addWidget(zEdit);
	}

	setLayout(m_layout);
}
