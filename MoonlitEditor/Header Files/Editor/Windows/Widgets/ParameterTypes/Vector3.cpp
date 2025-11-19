#include "Vector3.h"

#include <glm/glm.hpp>
#include <qlayout.h>

Vector3ParameterEditor::Vector3ParameterEditor(const ParameterRepositoryEntry& _entry, QWidget* _parent)
	: ParameterEditor(_entry, _parent)
{
	SetUpUI();
}

Vector3ParameterEditor::~Vector3ParameterEditor()
{
	delete m_validator;
	for (int i = 0; i < 3; ++i)
	{
		delete m_lineEdits[i];
		delete m_labels[i];
	}
}

void Vector3ParameterEditor::AddToLayout(QLayout* _layout)
{
	for (int i = 0; i < 3; ++i)
	{
		_layout->addWidget(m_labels[i]);
		_layout->addWidget(m_lineEdits[i]);
	}
}

void Vector3ParameterEditor::SetUpUI()
{
	QRegularExpression* regex = new QRegularExpression("^[\-0-9,\.]*$");
	QRegularExpressionValidator* validator = new QRegularExpressionValidator();
	validator->setRegularExpression(*regex);

	glm::vec3* data = (glm::vec3*)(m_entry.Data);

	for (int i = 0; i < 3; ++i)
	{
		float value = (*data)[i];

		m_labels[i] = new QLabel(this);
		m_lineEdits[i] = new QLineEdit(this);
		m_lineEdits[i]->setValidator(validator);
		m_lineEdits[i]->setText(QString::fromStdString(ParameterEditorHelper::GetPrettiestFloat(value)));

		connect(m_lineEdits[i], &QLineEdit::editingFinished, this, [data, i, this]()
			{
				std::istringstream stream(m_lineEdits[i]->text().toStdString());
				stream >> (*data)[i];

				// Update the text to ensure proper formatting
				m_lineEdits[i]->setText(QString::fromStdString(ParameterEditorHelper::GetPrettiestFloat((*data)[i])));
				
				// Notify that the parameter has changed
				OnParameterChanged.Invoke(this);
			});
	}

	m_labels[0]->setText("X:");
	m_labels[1]->setText("Y:");
	m_labels[2]->setText("Z:");

	delete regex;
}
