#include "Windows/SceneViewWindow.h"

#include "Engine/MoonlitEngine.h"
#include <QtCore/qtimer.h>

SceneViewWindow::SceneViewWindow(MoonlitEditor* _editor, MoonlitEngine* _engine) : EditorWindowBase(_editor)
{
	//m_engine = _engine;
	//The attribute and resize are needed to make sure the docked widget has a its own HWND
	//And with a valid size before show/creation
	setAttribute(Qt::WA_NativeWindow);
	resize(1920, 1080);
	show();
	createWinId();

	m_windowHandle = (HWND) effectiveWinId();

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
