#pragma once

#include "vulkan/vulkan.hpp"
#include "Inputs/InputManager.h"
#include "common.h"
#include "Material.h"
#include "DrawBuffer.h"

#include "RendererContext.h"
#include "Renderer/RendererDeviceManager.h"

class Camera;

class RendererContext;
class VulkanEngine;
class RenderTarget;

constexpr int TEXTURE_DESCRIPTOR_COUNT = 16;

class CameraInputHandler : public InputHandler
{
private:
	Camera* m_camera;
public:
	CameraInputHandler(Camera* _camera) : m_camera(_camera) {}

	void HandleMouseMoveInput(int _deltaX, int _deltaY) override;
	void HandleMouseInput(MOUSE_KEY _key, bool _keyDown) override;
	void HandleKeyboardInput(KEYBOARD_KEY _key, bool _keyDown) override;
	void HandleGamepadInput(GAMEPAD_KEY _key, bool _keyDown) override;
	void HandleGamepadAxis(GAMEPAD_KEY _key, float _x, float _y) override;
};

class MOONLIT_API Renderer
{
public:
	Renderer();

	void Init(ContextInfo& _info, std::vector<const char*> requiredExtensions);
	void Cleanup();

	void LoadMesh(std::string name);

	void Render();

	//Getters
	vk::RenderPass GetRenderPass() const { return m_mainRenderPass; }
	vk::Device GetDevice() const { return m_device; }
	vk::PhysicalDevice GetPhysicalDevice() const { return m_physicalDevice; }
	vk::Queue GetGraphicsQueue() const { return m_deviceManager->GetDefaultQueue(); }
	vk::CommandPool GetMainCommandPool() const { return m_mainCommandPool; }
	vk::DescriptorSetLayout GetUboDescriptorSetLayout() const { return m_uboDescriptorSetLayout; }
private:
	RendererContext m_context;
	RendererDeviceManager* m_deviceManager;

	std::vector<RenderTarget*> m_renderTargets;

	vk::Instance m_instance;
	vk::Device m_device;
	vk::PhysicalDevice m_physicalDevice;

	float m_timeStampPeriods;

	bool test = false;

	CameraInputHandler* m_inputHandler;

	Material* m_baseMaterial = nullptr;

	vk::RenderPass m_mainRenderPass;

	uint32_t m_framesInFlight = 3;
	uint32_t m_currentFrame = 0;

	std::vector<Camera*> m_cameras;

	bool m_windowClosed = false;

	std::vector<DrawBuffer> m_drawBuffers;

	vk::CommandPool m_mainCommandPool;
	vk::DescriptorSetLayout m_uboDescriptorSetLayout;

#pragma region Debug
#ifdef RENDER_DEBUG_INFORMATION_QUERY
	vk::QueryPool m_timestampQueryPool;

	void InitQueryPool();
#endif
#pragma endregion

#pragma region SwapchainParameters
	bool m_swapchainOutOfDate = false;
#pragma endregion

#pragma region Buffers

#pragma endregion

	void InitContext(ContextInfo& _info, std::vector<const char*> requiredExtensions);
	void InitVulkan();
	void InitRenderer();

	void CreateRenderPasses(vk::Format _format);

#pragma region Inputs
	void HandleWindowEvents(WINDOW_EVENT _event, void* _data);
#pragma endregion
	void RecordCommandBuffer(vk::CommandBuffer& _buffer, int _imageIndex);
	void CreateUboDescriptorSetLayout();
};