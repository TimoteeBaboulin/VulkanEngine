#include "Editor/Windows/General/SceneViewWindow.h"
#include "Editor/MoonlitEditor.h"
#include "Engine/MoonlitEngine.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/RenderTarget.h"
#include <QtCore/qtimer.h>
#include <QtGui/qevent.h>

#include "Debug/Logger.h"


SceneViewWindow::SceneViewWindow(QWidget* _parent) : 
	EditorWindowBase(_parent), m_camera(glm::vec3(0.0f, 0.0f, 40.0f), glm::vec3(0.0, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f))
{
	SetQtData();

	m_inputHandler = new SceneViewInputHandler(m_camera);

	m_engine = &MoonlitEditor::Editor->GetEngine();
	m_renderTarget = m_engine->Renderer->AddRenderTarget(m_windowHandle, &m_camera);

	m_resizeTimer = new QTimer(this);
	m_resizeTimer->setInterval(50);
	m_resizeTimer->setSingleShot(true);
	connect(m_resizeTimer, &QTimer::timeout, this, [this]() 
		{
			StopResizing();
		});

	//TODO: Handle scene input
	//m_inputHandler = new SceneViewInputHandler(&m_camera);
	//InputManager::GetInstance()->AddInputHandler(m_inputHandler);
}

bool SceneViewWindow::event(QEvent* _event)
{
	if (_event->type() == QEvent::Resize)
	{
		StartResizing();
	}

	return QDockWidget::event(_event);
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
	//TODO: Add connect to update timer here
}

void SceneViewWindow::StartResizing()
{
	if (!m_renderTarget)
		return;
	m_renderTarget->Pause(true);
	m_resizeTimer->start();
}

void SceneViewWindow::StopResizing()
{
	if (!m_renderTarget)
		return;
	m_renderTarget->Pause(false);
}


