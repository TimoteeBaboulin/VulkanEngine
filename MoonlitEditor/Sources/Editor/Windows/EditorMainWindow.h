#pragma once

#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qmenubar.h>

#include <DockManager.h>

class  EditorMainWindow : public QMainWindow
{
public:
	EditorMainWindow(size_t _width, size_t _height);
	//bool event(QEvent* _event) override;

	static bool TryGetInstance(EditorMainWindow*& _window);

private:
	void SetupConnections();
	void SetupUI();

	static EditorMainWindow* m_instance;

	size_t m_width, m_height;

	QMenuBar* m_menuBar;
	QMenu* m_profilerMenu;
	QAction* m_openProfilerAction;
};