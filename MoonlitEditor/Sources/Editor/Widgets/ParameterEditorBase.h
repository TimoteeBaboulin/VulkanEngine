#pragma once

#include <qwidget.h>
#include <qlabel.h>

#include "Engine/Component/ObjectBehaviour.h"

class ParameterEditorBase : public QWidget
{
public:
	ParameterEditorBase(const ParameterBase* _parameter, Moonlit::ObjectBehaviour* _behaviour);
	ParameterEditorBase(const ParameterEditorBase& _toCopy) = delete;

	~ParameterEditorBase() noexcept(true) override;

protected:
	virtual void SetParameterUI() {};
	void SetUI();

	const ParameterBase* m_parameterBase;
	Moonlit::ObjectBehaviour* m_behaviour;
	QLayout* m_layout;
	QLabel* m_name;
};