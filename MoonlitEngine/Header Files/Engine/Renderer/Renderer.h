#pragma once

#include "Engine/Inputs/InputManager.h"
#include "common.h"

#include <string>

struct Camera;

class RendererContext;
class RendererDeviceManager;
class VulkanEngine;
class RenderTarget;
class DrawBuffer;
namespace vk
{
	class Instance;
}

constexpr int TEXTURE_DESCRIPTOR_COUNT = 16;

/// <summary>
/// Main rendering class, this is the rendering module's entry point.
/// </summary>
class __declspec(dllexport) MoonlitRenderer
{
public:
	MoonlitRenderer();
	~MoonlitRenderer();

	void Init(struct ContextInfo& _info, std::vector<const char*> requiredExtensions);
	void Cleanup();
	uint32_t AddMeshInstance(std::shared_ptr<MeshData> _mesh,
		std::vector<std::shared_ptr<Image>> _textures,
		glm::mat4x4 _model);
	void UpdateInstanceModel(uint32_t _instanceId, glm::mat4x4 _model);
	void RemoveMeshInstance(uint32_t _instanceId);
	RenderTarget* AddRenderTarget(void* _handle, Camera* _camera);

	[[deprecated("This function is deprecated in favor of meshrenderers")]]
	void LoadMesh(std::string name);

	void Render();
private:
	RendererContext* m_context;
	RendererDeviceManager* m_deviceManager;

	std::vector<RenderTarget*> m_renderTargets;

	vk::Instance* m_instance;

	float m_timeStampPeriods;

	bool test = false;

	uint32_t m_framesInFlight = 3;
	uint32_t m_currentFrame = 0;

	std::vector<Camera*> m_cameras;

	bool m_windowClosed = false;

	std::vector<DrawBuffer*> m_drawBuffers;
	//vk::CommandPool m_mainCommandPool;

#pragma region Debug
#ifdef RENDER_DEBUG_INFORMATION_QUERY
	//vk::QueryPool m_timestampQueryPool;

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