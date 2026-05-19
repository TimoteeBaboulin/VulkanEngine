#include "MoonlitEditor.h"

#include <iostream>

#include <QtCore/qtimer.h>
#include <QtWidgets/qapplication.h>

#include <DockManager.h>

#include "EditorPaletter.h"
#include "Windows/EditorMainWindow.h"
#include "Windows/Core/SceneViewWindow.h"
#include "Windows/Core/FileExplorer.h"
#include "Windows/Core/SceneHierarchy.h"
#include "Windows/Core/ObjectEditor.h"
#include "Engine/MoonlitEngine.h"
#include "Engine/Inputs/InputManager.h"

#include "Debug/Profiler.h"
#include "Debug/Logger.h"

#include "Windows/DefaultDockManager.h"
#include "Engine/Component/GameObject.h"

#include "Engine/Events/EventUtility.h"
#include "Engine/Modules/ModuleManager.h"
#include "Engine/ResourceManagement/ResourceManager.h"
#include "Utility/CommandLineParser.h"

constexpr int DefaultEditorWidth = 860;
constexpr int DefaultEditorHeight = 540;

MoonlitEditor* MoonlitEditor::Editor = nullptr;

void GameobjectChangedTest(GameObject* _obj)
{
	if (_obj == nullptr)
	{
		Moonlit::Debug::Logger::LogInfo("Selected GameObject changed to nullptr");
		return;
	}
	Moonlit::Debug::Logger::LogInfo("Selected GameObject changed to " + _obj->GetName());
}

MoonlitEditor::MoonlitEditor()
{
	Editor = this;

	ads::CDockManager::setConfigFlags(ads::CDockManager::DefaultOpaqueConfig);
	ads::CDockManager::setConfigFlag(ads::CDockManager::EqualSplitOnInsertion, true);
	ads::CDockManager::setConfigFlag(ads::CDockManager::RetainTabSizeWhenCloseButtonHidden, true);

	//Needed for application initialization
	try
	{
		int argc = 1;
		char* argv[] = {(char*)"app", nullptr};
		m_app = new QApplication(argc, argv);

		std::string projectArg;
		std::filesystem::path projectRoot;
		if (Moonlit::Editor::CommandLineParser::TryGetValue("mlproject", projectArg))
		{
			projectRoot = std::filesystem::path(projectArg).parent_path();
			LOG_INFO("MoonlitEditor - Project root: " + projectRoot.string());
		}
		else
		{
			projectRoot = std::filesystem::path(m_app->applicationDirPath().toStdString());
		}
		m_projectPaths.Init(projectRoot);

		QFile styleSheet(QString::fromStdString(
			(m_projectPaths.GetResourcesPath() / "EditorStyles/darkstyle.qss").string()
		));
		if (styleSheet.open(QFile::ReadOnly | QFile::Text)) {
			QTextStream stream(&styleSheet);
			m_app->setStyleSheet(stream.readAll());
		}
	} catch (std::exception& e)
	{
		LOG_ERROR(e.what());
	}

	//Create the main window
	m_mainWindow = new EditorMainWindow(DefaultEditorWidth, DefaultEditorHeight);
	m_dockManager = new DefaultDockManager(m_mainWindow);

	HWND mainHandle = (HWND)m_mainWindow->effectiveWinId();

	// Engine requires a low level HWND to avoid breaking Qt's systems
	m_engine = new Moonlit::MoonlitEngine((void*)mainHandle);
	m_engine->Init();
	ReloadModules();
	Moonlit::ResourceManagement::ResourceManager::LoadResourcesFromDirectory(
		m_projectPaths.GetResourcesPath().string()
	);

	LoadDefaultLayout();

	Moonlit::Inputs::InputManager::Get().LinkQtApplication(m_app);

	m_app->processEvents();

	m_app->exec();
}

void MoonlitEditor::ReloadModules() {
	std::filesystem::path modulePath = m_projectPaths.GetModulesPath();
	std::filesystem::path tempPath = m_projectPaths.GetTempModulePath();
	std::filesystem::path rootPath = m_projectPaths.GetProjectRoot();

	Moonlit::MoonlitEngine& engine = Moonlit::MoonlitEngine::Get();
	Moonlit::ModuleManager &moduleManager = Moonlit::ModuleManager::Get();
	Moonlit::Tasks::WorkerManager& workerManager = *engine.GetMainWorkerManager();

	workerManager.drain();

	nlohmann::json sceneJson;
	engine.GetScene().SaveJson(sceneJson);

	engine.UnloadScene();
	moduleManager.UnloadAllModules();

	if (std::filesystem::exists(tempPath)) {
		std::filesystem::remove_all(tempPath);
	}
	std::filesystem::create_directory(tempPath);

	if (!std::filesystem::exists(modulePath) || !std::filesystem::is_directory(modulePath)) {
		LOG_WARNING("Modules directory does not exist, skipping module loading");
		return;
	}

	workerManager.restart();

	for (auto moduleFile: std::filesystem::recursive_directory_iterator(modulePath)) {
		if (moduleFile.path().extension() == ".dll") {

			std::shared_ptr<Moonlit::Tasks::Task> task = std::make_shared<Moonlit::Tasks::Task>([&tempPath, moduleFile]() {
				std::filesystem::path copyPath = tempPath/moduleFile.path().filename();
				std::filesystem::copy_file(moduleFile.path(), copyPath);
				Moonlit::ModuleManager::Get().LoadModule(copyPath);
			});

			workerManager.addTask(task);
			LOG_INFO("Scheduled loading of module " + moduleFile.path().string());
		}
	}

	workerManager.drain();
	workerManager.restart();

	engine.CreateSceneFromJson(sceneJson);
}

void MoonlitEditor::LoadDefaultLayout()
{
	m_editorWindows.push_back(new SceneView(m_dockManager));

	SceneView* sceneView = static_cast<SceneView*>(m_editorWindows.back());

	m_editorWindows.push_back(new FileExplorer(m_dockManager));

	m_editorWindows.push_back(new SceneHierarchy(m_dockManager));
	m_editorWindows.push_back(new ObjectInspector(*m_dockManager));

	m_updateCallback = std::bind(&Moonlit::MoonlitEngine::Update, m_engine);

	m_updateTimer = new QTimer(m_mainWindow);
	m_updateTimer->setInterval(16);
	m_updateTimer->setSingleShot(false);
	m_updateTimer->start();

	QObject::connect(m_updateTimer, &QTimer::timeout, m_mainWindow, m_updateCallback);
}