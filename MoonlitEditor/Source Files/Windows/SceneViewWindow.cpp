#include "Windows/SceneViewWindow.h"

#include "Engine/MoonlitEngine.h"
#include <QtCore/qtimer.h>

SceneViewWindow::SceneViewWindow(MoonlitEditor* _editor, MoonlitEngine* _engine) : EditorWindowBase(_editor)
{
	SetQtData();

	//This code will need to be removed to be played in the editor instead
	if (m_engine)
	{
		m_updateTimer = new QTimer(this);
		m_updateTimer->setSingleShot(false);
		m_updateTimer->setInterval(0);
		
		m_updateCallback = std::bind(&MoonlitEngine::Update, m_engine);
		connect(m_updateTimer, &QTimer::timeout, this, m_updateCallback);
	}
}

void SceneViewWindow::SetQtData()
{
	//setDockLocation(Qt::DockWidgetArea::TopDockWidgetArea);
	// This should be done automatically by Qt
	//setFeatures(QDockWidget::DockWidgetFeature::DockWidgetFloatable | QDockWidget::DockWidgetFeatures::set);

	//The attribute and resize are needed to make sure the docked widget has a its own HWND
	//And with a valid size before show/creation
	m_containerWidget = new QWidget(this);
	m_containerWidget->resize(1920, 1080);
	m_containerWidget->setAttribute(Qt::WA_NativeWindow);
	m_containerWidget->show();
	m_containerWidget->createWinId();
	m_windowHandle = (HWND)m_containerWidget->effectiveWinId();

	setWidget(m_containerWidget);

	show();

	//Check if the dockwidget don't already resize its content
}
