#include "MoonlitEditor.h"

#include <iostream>

#include <QtCore/qtimer.h>
#include <QtWidgets/qapplication.h>

#include "Windows/EditorMainWindow.h"
#include "Windows/SceneViewWindow.h"
#include "Engine/MoonlitEngine.h"



constexpr int DefaultEditorWidth = 1920;
constexpr int DefaultEditorHeight = 1080;

MoonlitEditor::MoonlitEditor()
{
	//Needed for application initialization
	char** argv = nullptr;
	int argc = 0;
	m_app = new QApplication(argc, argv);

	//Create the main window
	m_mainWindow = new EditorMainWindow(DefaultEditorWidth, DefaultEditorHeight);

	//By default, the main window already has a scene view docked to the left
	m_editorWindows.push_back(new SceneViewWindow(this, nullptr));
	SceneViewWindow* sceneView = static_cast<SceneViewWindow*>(m_editorWindows.back());
	m_mainWindow->addDockWidget(Qt::LeftDockWidgetArea, sceneView);

	//Create the engine now since it needs the window handle of the scene view
	//TODO: Make the engine surface handling more flexible and update the editor accordingly
	m_engine = new MoonlitEngine((void*)sceneView->GetWindowHandle());
	m_engine->Init();
	m_updateCallback = std::bind(&MoonlitEngine::Update, m_engine);

	m_updateTimer = new QTimer(m_mainWindow);
	m_updateTimer->setInterval(16);
	m_updateTimer->setSingleShot(false);
	m_updateTimer->start();
	sceneView->connect(m_updateTimer, &QTimer::timeout, sceneView, m_updateCallback);

	m_app->processEvents();

	m_app->exec();
}
