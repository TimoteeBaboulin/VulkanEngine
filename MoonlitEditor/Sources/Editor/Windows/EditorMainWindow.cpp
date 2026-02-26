#include "EditorMainWindow.h"
#include "Debug/ProfilerWindow.h"

#include "../../../../MoonlitEngine/Sources/Debug/Logger.h"

EditorMainWindow* EditorMainWindow::m_instance = nullptr;

EditorMainWindow::EditorMainWindow(size_t _width, size_t _height) : QMainWindow(), m_width(_width), m_height(_height)
{
    //Having multiple Main Windows could mean 2 editor classes are created for the same process.
    //This is not currently allowed as this might cause issues later down the line.
    if (m_instance != nullptr)
    {
		throw std::runtime_error("An instance of EditorMainWindow already exists.");
    }

    m_instance = this;

	SetupUI();
    SetupConnections();
}

bool EditorMainWindow::TryGetInstance(EditorMainWindow*& _window)
{
    if (m_instance == nullptr)
    {
        return false;
    }

	_window = m_instance;
    return true;
}

void EditorMainWindow::SetupConnections()
{
    connect(m_openProfilerAction, &QAction::triggered, this, []()
        {
			LOG_INFO("Opening Profiler Window...");
            ProfilerWindow* window = new ProfilerWindow();
        });
}

void EditorMainWindow::SetupUI()
{
    setWindowFlag(Qt::WindowFullscreenButtonHint, true);
    setWindowFlag(Qt::WindowMinMaxButtonsHint, true);
    setWindowFlag(Qt::WindowMaximizeButtonHint, true);
    setWindowFlag(Qt::WindowMinimizeButtonHint, true);

    //Make sure to resize the window to the specified width and height.
    resize(m_width, m_height);

    m_menuBar = new QMenuBar(this);
	setMenuBar(m_menuBar);

    m_profilerMenu = new QMenu("Profiler", this);
    m_menuBar->addMenu(m_profilerMenu);

	m_openProfilerAction = m_profilerMenu->addAction("Open Profiler");

    showMaximized();
}