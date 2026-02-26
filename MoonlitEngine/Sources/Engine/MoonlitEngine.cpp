#include "MoonlitEngine.h"
#include "Renderer/CustomVulkanImport.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererContext.h"
#include "ResourceManagement/ResourceManager.h"
#include "Scene/Scene.h"

#include "Debug/Logger.h"
#include "Debug/Profiler.h"
#include "Engine/Component/BehaviourRegistry.h"

#include "Debug/Exceptions/MoonlitExceptions.h"
#include "Engine/Modules/ModuleManager.h"

#include <filesystem>

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

void Moonlit::MoonlitEngine::Init()
{
	ResourceManagement::ResourceManager::Instance();
	ModuleManager& moduleManager = ModuleManager::Get();
	moduleManager.LoadModulesFromDirectory("./Modules");

	m_activeScene = new Scene();
	if (!m_activeScene)
	{
		LOG_ERROR("MoonlitEngine Init: Failed to create active scene");
		return;
	}
}
void Moonlit::MoonlitEngine::Update()
{
	// Update the renderer
	Renderer->Render();
}
