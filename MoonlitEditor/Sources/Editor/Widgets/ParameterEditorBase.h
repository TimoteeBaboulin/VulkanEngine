#pragma once

#include <qwidget.h>
#include <qlabel.h>

#include "Engine/Component/ObjectBehaviour.h"

class ParameterEditorBase : public QWidget
{
public:
	ParameterEditorBase(const ParameterBase* _parameter, Moonlit::ObjectBehaviour* _behaviour, QWidget* _parent = nullptr);
	ParameterEditorBase(const ParameterEditorBase& _toCopy) = delete;

	void SetUI();

	~ParameterEditorBase() noexcept(true) override;

protected:
	virtual void SetParameterUI() {};

	const ParameterBase* m_parameterBase;
	Moonlit::ObjectBehaviour* m_behaviour;
	QLayout* m_layout;
	QLabel* m_name;
};