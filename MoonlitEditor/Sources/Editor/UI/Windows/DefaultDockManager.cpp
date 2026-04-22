#include "DefaultDockManager.h"

DefaultDockManager::DefaultDockManager(QWidget* _parent)
{
	if (_parent)
	{
		m_dockManager = new ads::CDockManager(_parent);
	}
	else
	{
		m_dockManager = new ads::CDockManager();
	}
}

DefaultDockManager::~DefaultDockManager()
{
	for (DockWidgetPair& pair : m_dockWidgets)
	{
		m_dockManager->removeDockWidget(pair.second);
	}

	m_dockWidgets.clear();

	delete m_dockManager;
}

void DefaultDockManager::AddWidget(QWidget* _widget, const char* _title, ads::DockWidgetArea _area)
{
	DockWidgetPair pair = std::make_pair(_widget, m_dockManager->createDockWidget(_title));
	pair.second->setWidget(_widget);
	m_dockManager->addDockWidget(_area, pair.second);
	m_dockWidgets.emplace_back(pair);
}

void DefaultDockManager::RemoveWidget(QWidget* _widget)
{
	auto it = std::find_if(m_dockWidgets.begin(), m_dockWidgets.end(), [_widget](auto obj)
		{
			return obj.first == _widget;
		});

	if (it != m_dockWidgets.end())
	{
		m_dockManager->removeDockWidget(it->second);
		m_dockWidgets.erase(it);
	}
}
