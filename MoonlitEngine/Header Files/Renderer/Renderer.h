#pragma once
#include "DrawBuffer.h"
#include "Inputs/InputManager.h"

#include "Renderer/RendererDeviceManager.h"
#include "RendererContext.h"

struct Camera;

class RendererContext;
class VulkanEngine;
class RenderTarget;

constexpr int TEXTURE_DESCRIPTOR_COUNT = 16;

class CameraInputHandler : public InputHandler
{
private:
	Camera* m_camera;
	bool m_mouseHeld;

public:
	CameraInputHandler(Camera* _camera) : m_camera(_camera), m_mouseHeld(false) {}

	void HandleMouseMoveInput(int _deltaX, int _deltaY) override;
	void HandleMouseInput(MOUSE_KEY _key, bool _keyDown) override;
	void HandleKeyboardInput(KEYBOARD_KEY _key, bool _keyDown) override;
	void HandleGamepadInput(GAMEPAD_KEY _key, bool _keyDown) override;
	void HandleGamepadAxis(GAMEPAD_KEY _key, float _x, float _y) override;
};

class __declspec(dllexport) Renderer
{
public:
	Renderer();

	void Init(ContextInfo& _info, std::vector<const char*> requiredExtensions);
	void Cleanup();
	void AddRenderTarget(void* _handle, Camera* _camera);

	void LoadMesh(std::string name);

	void Render();

	//Getters
	//vk::CommandPool GetMainCommandPool() const { return m_mainCommandPool; }
private:
	RendererContext m_context;
	RendererDeviceManager* m_deviceManager;

	std::vector<RenderTarget*> m_renderTargets;

	vk::Instance m_instance;

	float m_timeStampPeriods;

	bool test = false;

	CameraInputHandler* m_inputHandler;

	uint32_t m_framesInFlight = 3;
	uint32_t m_currentFrame = 0;

	std::vector<Camera*> m_cameras;

	bool m_windowClosed = false;

	std::vector<DrawBuffer> m_drawBuffers;
	//vk::CommandPool m_mainCommandPool;

#pragma region Debug
#ifdef RENDER_DEBUG_INFORMATION_QUERY
	vk::QueryPool m_timestampQueryPool;

	void InitQueryPool();
#endif
#pragma endregion

#pragma region SwapchainParameters
	bool m_swapchainOutOfDate = false;
#pragma endregion

	void InitContext(ContextInfo& _info, std::vector<const char*> requiredExtensions);
	void InitVulkan();
	void InitRenderer();

	//void CreateRenderPasses(vk::Format _format);

#pragma region Inputs
	void HandleWindowEvents(WINDOW_EVENT _event, void* _data);
#pragma endregion
	//void RecordCommandBuffer(vk::CommandBuffer& _buffer, int _imageIndex);
	//void CreateUboDescriptorSetLayout();
};