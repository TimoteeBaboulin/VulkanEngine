#pragma once
#include "DrawBuffer.h"
#include "Engine/Inputs/InputManager.h"

#include "RendererDeviceManager.h"
#include "RendererContext.h"

struct Camera;

class RendererContext;
class VulkanEngine;
class RenderTarget;

constexpr int TEXTURE_DESCRIPTOR_COUNT = 16;

class __declspec(dllexport) Renderer
{
public:
	Renderer();

	void Init(ContextInfo& _info, std::vector<const char*> requiredExtensions);
	void Cleanup();
	void AddMeshInstance(MeshInstance& _meshInstance);
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