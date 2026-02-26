#pragma once

#include "ParameterEditor.h"
#include <qlineedit.h>
#include <qlabel.h>
#include <qvalidator.h>

class Vector3ParameterEditor : public ParameterEditor
{
public:
	Vector3ParameterEditor(const Moonlit::ParameterRepositoryEntry& _entry, QWidget* _parent = nullptr);
	~Vector3ParameterEditor();

	void AddToLayout(QLayout* _layout) override;
	void RemoveFromLayout(QLayout* _layout) override;

private:
	void SetUpUI();

	QLineEdit* m_lineEdits[3];
	QLabel* m_labels[3];

	QRegularExpressionValidator* m_validator;
};