#include "Editor/MoonlitEditor.h"

#include <iostream>

#include <QtCore/qtimer.h>
#include <QtWidgets/qapplication.h>

#include <ADS/DockManager.h>

#include "Editor/Windows/EditorMainWindow.h"
#include "Editor/Windows/General/SceneViewWindow.h"
#include "Editor/Windows/General/FileExplorer.h"
#include "Editor/Windows/General/SceneHierarchy.h"
#include "Editor/Windows/General/ObjectEditor.h"
#include "Engine/MoonlitEngine.h"
#include "Engine/Inputs/InputManager.h"

#include "Debug/Profiler.h"
#include "Debug/Logger.h"

#include "Editor/Windows/DefaultDockManager.h"
#include "Engine/GameObject.h"

#include "Engine/Events/EventUtility.h"

constexpr int DefaultEditorWidth = 860;
constexpr int DefaultEditorHeight = 540;

MoonlitEditor* MoonlitEditor::Editor = new MoonlitEditor();

void GameobjectChangedTest(GameObject* _obj)
{
	if (_obj == nullptr)
	{
		Logger::LogInfo("Selected GameObject changed to nullptr");
		return;
	}
	Logger::LogInfo("Selected GameObject changed to " + _obj->GetName());
}

MoonlitEditor::MoonlitEditor()
{
	ads::CDockManager::setConfigFlags(ads::CDockManager::DefaultOpaqueConfig);

	GameObject* testObj = GameObject::Create();
	{
		ScopedEventSubscriber<GameObject*> subscriber(MoonlitEditor::OnSelectionChanged(), GameobjectChangedTest);
		MoonlitEditor::OnSelectionChanged().Invoke(nullptr, testObj);
	}
	MoonlitEditor::OnSelectionChanged().Invoke(nullptr, testObj);

	Editor = this;

	//Needed for application initialization
	char** argv = nullptr;
	int argc = 0;
	m_app = new QApplication(argc, argv);
	m_app->setStyle("Fusion");

	//Create the main window
	m_mainWindow = new EditorMainWindow(DefaultEditorWidth, DefaultEditorHeight);
	m_dockManager = new DefaultDockManager(m_mainWindow);

	HWND mainHandle = (HWND)m_mainWindow->effectiveWinId();

	// Engine requires a low level HWND to avoid breaking Qt's systems
	m_engine = new MoonlitEngine((void*)mainHandle);
	m_engine->Init();

	//By default, the main window already has a scene view docked to the left
	m_editorWindows.push_back(new SceneViewWindow(m_dockManager));
	
	SceneViewWindow* sceneView = static_cast<SceneViewWindow*>(m_editorWindows.back());

	m_editorWindows.push_back(new FileExplorer(m_dockManager));

	m_editorWindows.push_back(new SceneHierarchy(m_dockManager));
	m_editorWindows.push_back(new ObjectInspector(*m_dockManager));

	MoonlitEditor::OnSelectionChanged().Invoke(nullptr, testObj);

	m_updateCallback = std::bind(&MoonlitEngine::Update, m_engine);

	m_updateTimer = new QTimer(m_mainWindow);
	m_updateTimer->setInterval(16);
	m_updateTimer->setSingleShot(false);
	m_updateTimer->start();
	QObject::connect(m_updateTimer, &QTimer::timeout, m_mainWindow, m_updateCallback);
	sceneView->connect(m_updateTimer, &QTimer::timeout, sceneView, m_updateCallback);

	InputManager::GetInstance()->LinkQtApplication(m_app);

	m_app->processEvents();

	m_app->exec();
}
