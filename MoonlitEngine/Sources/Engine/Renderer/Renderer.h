#pragma once

#include "../Inputs/InputManager.h"
#include "../../common.h"

#include <string>

namespace vk
{
	class Instance;
}

namespace Moonlit
{
	struct Camera;
}

namespace Moonlit::Renderer
{
	class RendererContext;
	class RendererDeviceManager;
	class VulkanEngine;
	class RenderTarget;
	class DrawBuffer;

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
		void UpdateInstanceMesh(uint32_t _instanceId, std::shared_ptr<MeshData> _mesh);
		void RemoveMeshInstance(uint32_t _instanceId);
		RenderTarget* AddRenderTarget(void* _handle, Camera* _camera);

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

#pragma region Inputs
		void HandleWindowEvents(Moonlit::Inputs::WINDOW_EVENT _event, void* _data);
#pragma endregion
	};
}