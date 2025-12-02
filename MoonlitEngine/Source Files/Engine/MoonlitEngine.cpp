#include "Engine/MoonlitEngine.h"
#include <vulkan/vulkan.h>
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/RendererContext.h"
#include "ResourceManagement/ResourceManager.h"
#include "Engine/Scene/Scene.h"

#include "Debug/Logger.h"
#include "Debug/Profiler.h"
#include "ResourceManagement/Mesh.h"
#include "Engine/Components/BehaviourRegistry.h"

#include "Debug/Exceptions/MoonlitExceptions.h"

MoonlitEngine* MoonlitEngine::m_instance = nullptr;

MoonlitEngine::MoonlitEngine(void* _handle)
{
	m_instance = this;

	Logger::LogInfo("Initializing Moonlit Engine...");

	InputManager::InitManager(_handle);

	// Initialize the renderer
	Renderer = new MoonlitRenderer();
	ContextInfo contextInfo;
	contextInfo.name = "Moonlit Engine";
	Renderer->Init(contextInfo, {VK_KHR_SURFACE_EXTENSION_NAME, "VK_KHR_win32_surface"});

	Logger::LogInfo("Moonlit Engine initialized successfully.");

	try
	{
		LoadPlugin("DefaultPlugin.dll");
	}
	catch (PluginLoadException& e)
	{
		Logger::LogError("Error loading plugin: " + std::string(e.what()));
		if (e.Error() == PluginLoadError::REGISTER_FUNCTION_MISSING)
		{
			// Since this is the default plugin, freeing the library is our job
			// This is still a very serious case as the main components are unusable without this plugin
			FreeLibrary(e.PluginModule());
			return;
		}
		throw;
	}
}

void MoonlitEngine::LoadPlugin(std::string _name)
{
	std::string pluginPath = _name;
	HMODULE pluginHandle = LoadLibraryA(pluginPath.c_str());
	if (pluginHandle == NULL)
	{
		DWORD error = GetLastError();
		Logger::LogError("Failed to load plugin: " + _name + " Error: " + std::to_string(error));
		throw PluginLoadException(PluginLoadError::FILE_NOT_FOUND, _name);
		return;
	}

	FARPROC registerFunction = GetProcAddress(pluginHandle, "GetRegistry");
	if (!registerFunction)
	{
		Logger::LogError("Failed to find GetRegistry function in plugin: " + _name);
		throw PluginLoadException(PluginLoadError::REGISTER_FUNCTION_MISSING, _name, pluginHandle);
		return;
	}

	PluginRegistryFunction pluginRegister = reinterpret_cast<PluginRegistryFunction>(registerFunction);
	auto registries = pluginRegister();
	for (auto it = registries.begin(); it != registries.end(); it++)
	{
		BehaviourRegistry::RegisterBehaviour((*it).Name, (*it).CreateFunction);
	}
	LOG_INFO("Plugin loaded and behaviours registered: " + _name);
}

void MoonlitEngine::Init()
{
	ResourceManager::TryLoadResource<MeshData>("Meshes/barstool.gltf");
	ResourceManager::TryLoadResource<MeshData>("Meshes/Sniper_Final.fbx");
	ResourceManager::TryLoadResource<Image>("Textures/barstool_albedo.png");
	ResourceManager::TryLoadResource<Image>("Textures/Sniper.png");

	m_activeScene = new Scene();
}
void MoonlitEngine::Update()
{
	// Update the renderer
	Renderer->Render();
}