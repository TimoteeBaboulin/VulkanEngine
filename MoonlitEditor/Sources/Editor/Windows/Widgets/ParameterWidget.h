#pragma once

#include <qwidget.h>
#include <qlabel.h>

#include "../../../../../MoonlitEngine/Sources/Engine/Component/ObjectBehaviour.h"

class ParameterWidget : public QWidget
{
public:
	ParameterWidget(const Moonlit::ParameterRepositoryEntry& _entry, Moonlit::ObjectBehaviour* _behaviour);
	ParameterWidget(const ParameterWidget& _toCopy) = delete;

	~ParameterWidget() noexcept(true) override;

protected:
	void SetUI();

	const Moonlit::ParameterRepositoryEntry& m_entry;
	Moonlit::ObjectBehaviour* m_behaviour;
	QLayout* m_layout;
	QLabel* m_name;
};