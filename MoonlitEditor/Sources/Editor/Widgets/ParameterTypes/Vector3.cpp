#include "Vector3.h"

#include <glm/glm.hpp>
#include <qlayout.h>

#include "../ParameterEditorBase.h"

namespace Moonlit::Editor
{
	Vector3ParameterEditor::~Vector3ParameterEditor()
	{
		delete m_validator;
		for (int i = 0; i < 3; ++i)
		{
			delete m_lineEdits[i];
			delete m_labels[i];
		}
	}

	void Vector3ParameterEditor::SetParameterUI()
	{
		LOG_INFO("Vector3ParameterEditor::SetParameterUI");
		QRegularExpression* regex = new QRegularExpression("^[\-0-9,\.]*$");
		m_validator = new QRegularExpressionValidator();

		glm::vec3 data = m_parameter.Value();

		for (int i = 0; i < 3; ++i)
		{
			float value = data[i];

			m_labels[i] = new QLabel(this);
			m_lineEdits[i] = new QLineEdit(this);
			m_lineEdits[i]->setValidator(m_validator);
			m_lineEdits[i]->setText(QString::fromStdString(ParameterEditorHelper::GetPrettiestFloat(value)));

			connect(m_lineEdits[i], &QLineEdit::editingFinished, this, [i, this]()
				{
					std::istringstream stream(m_lineEdits[i]->text().toStdString());
					glm::vec3 vector = m_parameter.Value();
					stream >> vector[i];

					// Update the text to ensure proper formatting
					m_lineEdits[i]->setText(QString::fromStdString(ParameterEditorHelper::GetPrettiestFloat(vector[i])));

					// Update the new value
					ValueChanged(vector);
				});
		}

		m_labels[0]->setText("X:");
		m_labels[1]->setText("Y:");
		m_labels[2]->setText("Z:");

		for (int i = 0; i < 3; ++i)
		{
			m_layout->addWidget(m_labels[i]);
			m_layout->addWidget(m_lineEdits[i]);
		}

		delete regex;
	}
}