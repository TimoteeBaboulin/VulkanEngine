#pragma once

#include <ADS/DockManager.h>

class IDockManager
{
public:
	virtual void AddWidget(QWidget* _widget, const char* _title, ads::DockWidgetArea _area) = 0;
	virtual void RemoveWidget(QWidget* _widget) = 0;

protected:
	ads::CDockManager* m_dockManager = nullptr;
};