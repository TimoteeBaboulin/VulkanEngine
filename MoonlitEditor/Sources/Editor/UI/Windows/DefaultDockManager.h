#pragma once

#include "IDockManager.h"

using DockWidgetPair = std::pair<QWidget*, ads::CDockWidget*>;

class DefaultDockManager : public IDockManager
{
public:
	DefaultDockManager(QWidget* _parent = nullptr);

	// Prevent copy because we can't copy the widgets without causing unwanted behaviour

	DefaultDockManager(const DefaultDockManager& _other) = delete;
	DefaultDockManager& operator=(const DefaultDockManager& _other) = delete;

	// Prevent move semantics because we can't move the widgets without breaking layouts

	DefaultDockManager(DefaultDockManager&& _other) = delete;
	DefaultDockManager& operator=(DefaultDockManager&& _other) = delete;

	~DefaultDockManager();

	void AddWidget(QWidget* _widget, const char* _title, ads::DockWidgetArea _area) override;
	void RemoveWidget(QWidget* _widget) override;

private:
	std::vector<DockWidgetPair> m_dockWidgets;
};