#pragma once

#include "../ParameterEditor.h"
#include <qlineedit.h>
#include <qlabel.h>
#include <qvalidator.h>

#include <glm/vec3.hpp>
#include <QtWidgets/qwidget.h>

namespace Moonlit::Editor
{
	class Vector3ParameterEditor : public ParameterEditor<glm::vec3>
	{
	public:
		Vector3ParameterEditor(Parameter* _parameter, ObjectBehaviour* _behaviour, QWidget* _parent = nullptr) : ParameterEditor<glm::vec3>(_parameter, _behaviour, _parent) {};
		~Vector3ParameterEditor() override;

		void SetParameterUI() override;

	private:
		QLineEdit* m_lineEdits[3];
		QLabel* m_labels[3];

		QRegularExpressionValidator* m_validator;
	};
}
