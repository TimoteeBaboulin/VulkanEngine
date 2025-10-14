#include "Engine/MoonlitEngine.h"
#include "Renderer/Renderer.h"

#include "ResourceManagement/MeshBank.h"
#include "ResourceManagement/TextureBank.h"

#include "vulkan/vulkan.h"

MoonlitEngine::MoonlitEngine(void* _handle)
{
	std::cout << "Initializing Moonlit Engine..." << std::endl;

	InputManager::InitManager(_handle);

	// Initialize the renderer
	m_renderer = Renderer();
	ContextInfo contextInfo;
	RECT screenSize;
	GetClientRect((HWND)_handle, &screenSize);
	contextInfo.name = "Moonlit Engine";
	m_renderer.Init(contextInfo, {VK_KHR_SURFACE_EXTENSION_NAME, "VK_KHR_win32_surface"});

#ifdef _DEBUG
	std::cout << "Initialized Moonlit Engine!" << std::endl;
#endif
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
	std::cout << "Updating Moonlit Engine..." << std::endl;
	m_renderer.Render();
}