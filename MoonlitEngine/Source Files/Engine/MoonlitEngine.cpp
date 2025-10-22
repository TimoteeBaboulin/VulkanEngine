#include "Engine/MoonlitEngine.h"
#include <vulkan/vulkan.h>
#include "Engine/Renderer/Renderer.h"
#include "ResourceManagement/ResourceManager.h"
#include "Debug/Logger.h"

MoonlitEngine::MoonlitEngine(void* _handle)
{

	Logger::LogInfo("Initializing Moonlit Engine...");

	InputManager::InitManager(_handle);

	// Initialize the renderer
	m_renderer = Renderer();
	ContextInfo contextInfo;
	contextInfo.name = "Moonlit Engine";
	m_renderer.Init(contextInfo, {VK_KHR_SURFACE_EXTENSION_NAME, "VK_KHR_win32_surface"});

	Logger::LogInfo("Moonlit Engine initialized successfully.");
}

void MoonlitEngine::Init()
{

	ResourceManager::TryLoadResource<MeshData>("Meshes/barstool.gltf");
	ResourceManager::TryLoadResource<MeshData>("Meshes/Sniper_Final.fbx");
	ResourceManager::TryLoadResource<Image>("Textures/barstool_albedo.png");
	ResourceManager::TryLoadResource<Image>("Textures/Sniper.png");

	m_renderer.LoadMesh("barstool");
}
void MoonlitEngine::Update()
{
	// Update the renderer
	m_renderer.Render();
}