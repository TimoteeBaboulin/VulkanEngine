#include "Editor/MoonlitEditor.h"

#include <iostream>

#include <QtCore/qtimer.h>
#include <QtWidgets/qapplication.h>

#include "Editor/Windows/EditorMainWindow.h"
#include "Editor/Windows/General/SceneViewWindow.h"
#include "Editor/Windows/General/FileExplorer.h"
#include "Editor/Windows/General/SceneHierarchy.h"
#include "Engine/MoonlitEngine.h"
#include "Engine/Inputs/InputManager.h"

#include "Debug/Profiler.h"
#include "Debug/Logger.h"

constexpr int DefaultEditorWidth = 1920;
constexpr int DefaultEditorHeight = 1080;

MoonlitEditor* MoonlitEditor::Editor = new MoonlitEditor();

MoonlitEditor::MoonlitEditor()
{
	Editor = this;

	//Needed for application initialization
	char** argv = nullptr;
	int argc = 0;
	m_app = new QApplication(argc, argv);
	m_app->setStyle("Fusion");

	//Create the main window
	m_mainWindow = new EditorMainWindow(DefaultEditorWidth, DefaultEditorHeight);
	HWND mainHandle = (HWND)m_mainWindow->effectiveWinId();

	// Engine requires a low level HWND to avoid breaking Qt's systems
	m_engine = new MoonlitEngine((void*)mainHandle);
	m_engine->Init();

	//By default, the main window already has a scene view docked to the left
	m_editorWindows.push_back(new SceneViewWindow(m_mainWindow));
	SceneViewWindow* sceneView = static_cast<SceneViewWindow*>(m_editorWindows.back());
	m_mainWindow->addDockWidget(Qt::TopDockWidgetArea, sceneView);

	m_editorWindows.push_back(new FileExplorer(m_mainWindow));
	m_mainWindow->addDockWidget(Qt::BottomDockWidgetArea, m_editorWindows.back());

	m_editorWindows.push_back(new SceneHierarchy(m_mainWindow));
	m_mainWindow->addDockWidget(Qt::RightDockWidgetArea, m_editorWindows.back());

	m_updateCallback = std::bind(&MoonlitEngine::Update, m_engine);

	m_updateTimer = new QTimer(m_mainWindow);
	m_updateTimer->setInterval(16);
	m_updateTimer->setSingleShot(false);
	m_updateTimer->start();
	QObject::connect(m_updateTimer, &QTimer::timeout, m_mainWindow, m_updateCallback);
	sceneView->connect(m_updateTimer, &QTimer::timeout, sceneView, m_updateCallback);

	InputManager::GetInstance()->LinkQtApplication(m_app);

	m_app->processEvents();

	FrameSample frame = Profiler::Get().GetLastFrameSample();
	if (frame.stackSamples.size() > 0)
	{
		LOG_INFO("Profiler sample retrieved successfully.");
	}
	else
	{
		LOG_WARNING("No profiler samples available.");
	}

	m_app->exec();
}
