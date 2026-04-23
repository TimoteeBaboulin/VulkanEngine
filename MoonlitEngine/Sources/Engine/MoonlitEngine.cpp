#include "MoonlitEngine.h"
#include "Renderer/CustomVulkanImport.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererContext.h"
#include "ResourceManagement/ResourceManager.h"
#include "Scene/Scene.h"

#include "Debug/Logger.h"
#include "Debug/Profiler.h"
#include "Engine/Tasks/WorkerManager.h"
#include "Engine/Modules/ModuleManager.h"

#include <filesystem>
#include <iostream>

#include "Scene/SceneManager.h"

Moonlit::MoonlitEngine* Moonlit::MoonlitEngine::m_instance = nullptr;

Moonlit::MoonlitEngine::MoonlitEngine(void* _handle)
{
	m_instance = this;

	Debug::Logger::LogInfo("Initializing Moonlit Engine...");

	Inputs::InputManager::InitManager(_handle);

	// Initialize the renderer
	Renderer = new Renderer::MoonlitRenderer();
	Renderer::ContextInfo contextInfo;
	contextInfo.name = "Moonlit Engine";

	std::vector<const char*> requiredExtensions;
	requiredExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
	requiredExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
	//requiredExtensions.push_back(VK_EXT_HEADLESS_SURFACE_EXTENSION_NAME);

	Renderer->Init(contextInfo, requiredExtensions);

	Debug::Logger::LogInfo("Moonlit Engine initialized successfully.");
}

void Moonlit::MoonlitEngine::LoadModule(std::string _name)
{
	std::filesystem::path modulePath(_name);
	ModuleManager::Get().LoadModule(modulePath);
}

void SendData()
{
	std::cout << "MoonlitEngine Init: Sending data" << std::endl;
}

void Moonlit::MoonlitEngine::Init()
{
	ResourceManagement::ResourceManager::Get();
	ModuleManager& moduleManager = ModuleManager::Get();
	moduleManager.LoadModulesFromDirectory("./Modules");

	m_activeScene = new Scene();
	if (!m_activeScene)
	{
		LOG_ERROR("MoonlitEngine Init: Failed to create active scene");
		return;
	}

	Tasks::WorkerManager* manager = new Tasks::WorkerManager();

	std::vector<Tasks::TASK_FUNC> tasks;
	for (int i = 0; i < 10; i++)
	{
		tasks.push_back([i]() {
			LOG_INFO("Task " + std::to_string(i) + " is running");
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			LOG_INFO("Task " + std::to_string(i) + " is done");
		});
	}

	Tasks::CurrentWorkerManager->addTasks(tasks);
}

void Moonlit::MoonlitEngine::Update()
{
	static std::chrono::steady_clock::time_point lastFrameTime = std::chrono::steady_clock::now();

	auto now = std::chrono::steady_clock::now();
	float dt = std::chrono::duration<float>(now - lastFrameTime).count();
	lastFrameTime = now;

	// Start wth simulation
	if (m_isPlaying)
	{
		m_activeScene->Update(dt);
		m_activeScene->LateUpdate(dt);
	}

	// Then render
	if (m_isPlaying)
	{
		m_activeScene->PreRender();
		Renderer->Render();
		m_activeScene->PostRender();
	}
	else
	{
		Renderer->Render();
	}
}

void Moonlit::MoonlitEngine::LoadScene(const std::string& _path)
{
	if (m_activeScene)
	{
		UnloadScene();
	}

	m_activeScene = new Scene(_path);
	OnSceneLoaded(this, m_activeScene);
}

void Moonlit::MoonlitEngine::ReloadScene()
{
	if (!m_activeScene)
	{
		LOG_ERROR("MoonlitEngine ReloadScene: Failed to reload scene");
		return;
	}

	std::string path = m_activeScene->GetSavePath();
	if (path.empty())
	{
		LOG_ERROR("MoonlitEngine ReloadScene: Can't reload scene because scene isn't saved");
		return;
	}

	LoadScene(path);
}

void Moonlit::MoonlitEngine::UnloadScene()
{
	OnSceneUnloaded(this, m_activeScene);
	delete m_activeScene;
	m_activeScene = nullptr;
}
