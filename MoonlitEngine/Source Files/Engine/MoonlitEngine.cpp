#include "Engine/MoonlitEngine.h"
#include <vulkan/vulkan.h>
#include "Engine/Renderer/Renderer.h"
#include "ResourceManagement/ResourceManager.h"
#include "Engine/Scene/Scene.h"

#include "Debug/Logger.h"
#include "ResourceManagement/Mesh.h"
#include "Engine/Components/BehaviourRegistry.h"

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

	LoadPlugin("DefaultPlugin.dll");
}

void MoonlitEngine::LoadPlugin(std::string _name)
{
	std::string pluginPath = _name;
	HMODULE pluginHandle = LoadLibraryA(pluginPath.c_str());
	if (pluginHandle == NULL)
	{
		DWORD error = GetLastError();
		Logger::LogError(("Failed to load plugin: " + _name + " Error: " + std::to_string(error)).c_str());
		return;
	}

	FARPROC registerFunction = GetProcAddress(pluginHandle, "GetRegistry");
	if (!registerFunction)
	{
		Logger::LogError(("Failed to find GetRegistry function in plugin: " + _name).c_str());
		FreeLibrary(pluginHandle);
		return;
	}

	PluginRegistryFunction pluginRegister = reinterpret_cast<PluginRegistryFunction>(registerFunction);
	auto registries = pluginRegister();
	for (auto it = registries.begin(); it != registries.end(); it++)
	{
		BehaviourRegistry::RegisterBehaviour((*it).Name, (*it).CreateFunction);
	}
	LOG_INFO(("Plugin loaded and behaviours registered: " + _name).c_str());
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