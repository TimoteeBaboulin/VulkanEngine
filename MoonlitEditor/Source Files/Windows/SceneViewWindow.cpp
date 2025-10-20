#include "Windows/SceneViewWindow.h"
#include "MoonlitEditor.h"
#include "..\..\SceneViewInputHandler.h"
#include "Engine/MoonlitEngine.h"
#include <QtCore/qtimer.h>


SceneViewWindow::SceneViewWindow(MoonlitEditor* _editor, MoonlitEngine* _engine, QWidget* _parent) : 
	EditorWindowBase(_editor, _parent), m_camera(glm::vec3(20.0f, 30.0f, 35.0f), glm::vec3(1.0, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f))
{
	SetQtData();

	_editor->GetEngine().AddRenderTarget(m_windowHandle, &m_camera);

	//m_inputHandler = new SceneViewInputHandler(&m_camera);
	//InputManager::GetInstance()->AddInputHandler(m_inputHandler);

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
	//The attribute and resize are needed to make sure the docked widget has a its own HWND
	//And with a valid size before show/creation
	m_containerWidget = new QWidget();
	m_containerWidget->resize(1920, 1080);
	m_containerWidget->setMinimumSize(200, 200);
	m_containerWidget->setMaximumSize(1920, 1080);
	m_containerWidget->setAttribute(Qt::WA_NativeWindow);
	m_containerWidget->show();
	m_containerWidget->createWinId();
	QSizePolicy policy = m_containerWidget->sizePolicy();
	policy.setHorizontalStretch(1);
	policy.setHorizontalPolicy(QSizePolicy::Preferred);
	policy.setVerticalStretch(1);
	policy.setVerticalPolicy(QSizePolicy::Preferred);
	m_containerWidget->setSizePolicy(policy);

	m_windowHandle = (HWND)m_containerWidget->effectiveWinId();

	setWidget(m_containerWidget);

	show();
	//TODO: Find a way to prevent docking onto this widget
}
