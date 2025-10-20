#include "Engine/MoonlitEngine.h"
#include <vulkan/vulkan.h>
#include "Engine/Renderer/Renderer.h"
#include "ResourceManagement/MeshBank.h"
#include "ResourceManagement/TextureBank.h"
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
	MeshBank::Initialize();
	TextureBank::Initialize();

	MeshBank::Instance->TryLoad("Meshes/barstool.gltf");
	MeshBank::Instance->TryLoad("Meshes/Sniper_Final.fbx");

	TextureBank::Instance->TryLoad("Textures/barstool_albedo.png");

	m_renderer.LoadMesh("barstool");
}
void MoonlitEngine::Update()
{
	// Update the renderer
	m_renderer.Render();
}