#include "Editor/Windows/General/SceneViewWindow.h"
#include "Editor/MoonlitEditor.h"
#include "SceneViewInputHandler.h"
#include "Engine/MoonlitEngine.h"
#include <QtCore/qtimer.h>


SceneViewWindow::SceneViewWindow(QWidget* _parent) : 
	EditorWindowBase(_parent), m_camera(glm::vec3(20.0f, 30.0f, 35.0f), glm::vec3(1.0, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f))
{
	SetQtData();

	m_engine = &MoonlitEditor::Editor->GetEngine();
	m_engine->AddRenderTarget(m_windowHandle, &m_camera);

	//TODO: Handle scene input
	//m_inputHandler = new SceneViewInputHandler(&m_camera);
	//InputManager::GetInstance()->AddInputHandler(m_inputHandler);
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
