#include "Renderer.h"
#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

#include "RendererDeviceManager.h"
#include "RendererContext.h"
#include "RenderDevice.h"
#include "DrawBuffer.h"
#include "../../common.h"

#include "Material/Material.h"
#include "Camera.h"

#include "../ResourceManagement/ResourceManager.h"
#include "RenderTarget.h"

#include "../../Debug/Logger.h"
#include "Engine/ResourceManagement/ResourceHandle.h"

Moonlit::Renderer::MoonlitRenderer::MoonlitRenderer()
{
	m_context = new RendererContext();
}

Moonlit::Renderer::MoonlitRenderer::~MoonlitRenderer()
{
	Cleanup();
}

vk::ApplicationInfo GetAppInfo(const char* _appName)
{
	vk::ApplicationInfo appInfo{};
	appInfo.sType = vk::StructureType::eApplicationInfo;
	appInfo.pApplicationName = _appName;
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Moonlit Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_4;

	return appInfo;
}

void Moonlit::Renderer::MoonlitRenderer::InitContext(ContextInfo& _info, std::vector<const char*> requiredExtensions)
{
	m_context->Init();
	vk::ApplicationInfo appInfo = GetAppInfo(_info.name);
	m_instance = m_context->CreateInstance(appInfo, requiredExtensions.data(), requiredExtensions.size());
	m_deviceManager = new RendererDeviceManager(*m_instance);
	std::function<void(Moonlit::Inputs::WINDOW_EVENT, void*)> windowCallback = std::bind(&MoonlitRenderer::HandleWindowEvents, this, std::placeholders::_1, std::placeholders::_2);
}

void Moonlit::Renderer::MoonlitRenderer::InitVulkan()
{
	//TODO: Repare render profiling
	//m_timeStampPeriods = m_deviceManager->GetTimeStampPeriods();
}

void Moonlit::Renderer::MoonlitRenderer::InitRenderer()
{
#ifdef RENDER_DEBUG_INFORMATION_QUERY
	InitQueryPool();
#endif
}

void Moonlit::Renderer::MoonlitRenderer::Init(ContextInfo& _info, std::vector<const char*> requiredExtensions)
{
	InitContext(_info, requiredExtensions);
	InitVulkan();
	InitRenderer();
}

void Moonlit::Renderer::MoonlitRenderer::Cleanup()
{
	for (auto& [mat, buf] : m_drawBuffers)
		delete buf;
	m_drawBuffers.clear();
	m_renderTargets.clear();
	m_renderDevices.clear();
	delete m_deviceManager;
	delete m_context;
}

Moonlit::Renderer::RenderDevice& Moonlit::Renderer::MoonlitRenderer::GetOrCreateRenderDevice(DeviceData const& _deviceData)
{
	for (auto& rd : m_renderDevices)
	{
		if (rd->GetDeviceData().PhysicalDevice == _deviceData.PhysicalDevice)
			return *rd;
	}
	m_renderDevices.push_back(std::make_unique<RenderDevice>(_deviceData, *m_deviceManager));
	return *m_renderDevices.back();
}

Moonlit::Renderer::DrawBuffer* Moonlit::Renderer::MoonlitRenderer::GetOrCreateDrawBuffer(Material* _material)
{
	auto it = m_drawBuffers.find(_material);
	if (it != m_drawBuffers.end())
		return it->second;
	if (m_renderDevices.empty())
	{
		LOG_ERROR("MoonlitRenderer::GetOrCreateDrawBuffer - No render device available.");
		return nullptr;
	}
	DrawBuffer* newBuffer = new DrawBuffer(_material);
	m_drawBuffers.emplace(_material, newBuffer);
	return newBuffer;
}

Moonlit::Renderer::DrawBuffer* Moonlit::Renderer::MoonlitRenderer::FindOrCreateDrawBuffer(MaterialHandle _material)
{
	return GetOrCreateDrawBuffer(&(*_material));
}

uint32_t Moonlit::Renderer::MoonlitRenderer::AddMeshInstance(MaterialHandle _material,
	MeshHandle _mesh,
	std::vector<TextureHandle> _textures,
	glm::mat4x4 _model)
{
	if (!_material.IsValid())
	{
		LOG_ERROR("MoonlitRenderer::AddMeshInstance - Invalid material handle.");
		return UINT32_MAX;
	}
	DrawBuffer* buffer = FindOrCreateDrawBuffer(_material);
	uint32_t id = buffer->AddMeshInstance(_mesh, _textures, _model);
	m_instanceBufferMap[id] = buffer;
	return id;
}

void Moonlit::Renderer::MoonlitRenderer::UpdateInstanceModel(uint32_t _instanceId, glm::mat4x4 _model)
{
	auto it = m_instanceBufferMap.find(_instanceId);
	if (it == m_instanceBufferMap.end())
	{
		LOG_WARNING("Renderer::UpdateInstanceModel - Instance not found in buffer map.");
		return;
	}
	it->second->UpdateInstanceModel(_instanceId, _model);
}

void Moonlit::Renderer::MoonlitRenderer::UpdateInstanceMesh(uint32_t _instanceId, MeshHandle _mesh)
{
	auto it = m_instanceBufferMap.find(_instanceId);
	if (it == m_instanceBufferMap.end())
	{
		LOG_WARNING("Renderer::UpdateInstanceMesh - Instance not found in buffer map.");
		return;
	}
	it->second->UpdateInstanceMesh(_instanceId, _mesh);
}

void Moonlit::Renderer::MoonlitRenderer::UpdateInstanceTextures(uint32_t _instanceId, std::vector<TextureHandle> _textures)
{
	auto it = m_instanceBufferMap.find(_instanceId);
	if (it == m_instanceBufferMap.end())
	{
		LOG_WARNING("Renderer::UpdateInstanceTextures - Instance not found in buffer map.");
		return;
	}
	it->second->UpdateInstanceTextures(_instanceId, _textures);
}

uint32_t Moonlit::Renderer::MoonlitRenderer::UpdateInstanceMaterial(uint32_t _instanceId, MaterialHandle _material, MeshHandle _mesh, std::vector<TextureHandle> _textures)
{
	auto it = m_instanceBufferMap.find(_instanceId);
	if (it == m_instanceBufferMap.end())
	{
		LOG_WARNING("Renderer::UpdateInstanceMaterial - Instance not found in buffer map.");
		return _instanceId;
	}

	DrawBuffer* oldBuffer = it->second;
	glm::mat4x4 model = oldBuffer->GetInstanceModel(_instanceId);
	oldBuffer->RemoveMeshInstance(_instanceId);
	m_instanceBufferMap.erase(it);

	if (!_material.IsValid())
	{
		LOG_ERROR("MoonlitRenderer::UpdateInstanceMaterial - Invalid material handle.");
		return _instanceId;
	}
	DrawBuffer* newBuffer = FindOrCreateDrawBuffer(_material);
	uint32_t newId = newBuffer->AddMeshInstance(_mesh, _textures, model);
	m_instanceBufferMap[newId] = newBuffer;
	return newId;
}

void Moonlit::Renderer::MoonlitRenderer::RemoveMeshInstance(uint32_t _instanceId)
{
	auto it = m_instanceBufferMap.find(_instanceId);
	if (it == m_instanceBufferMap.end())
	{
		LOG_WARNING("Renderer::RemoveMeshInstance - Instance not found in buffer map.");
		return;
	}
	it->second->RemoveMeshInstance(_instanceId);
	m_instanceBufferMap.erase(it);
}

Moonlit::Renderer::RenderTarget* Moonlit::Renderer::MoonlitRenderer::AddRenderTarget(void* _handle, Camera* _camera)
{
	vk::Win32SurfaceCreateInfoKHR surfaceCreateInfo{};
	surfaceCreateInfo.sType = vk::StructureType::eWin32SurfaceCreateInfoKHR;
	surfaceCreateInfo.hinstance = GetModuleHandle(nullptr);
	surfaceCreateInfo.hwnd = (HWND)_handle;
	vk::SurfaceKHR surface = m_instance->createWin32SurfaceKHR(surfaceCreateInfo);

	DeviceData deviceData = m_deviceManager->GetOrCreateDeviceForSurface(surface);
	RenderDevice& renderDevice = GetOrCreateRenderDevice(deviceData);
	m_renderTargets.push_back(std::make_unique<RenderTarget>(3, (HWND)_handle, surface, *m_instance, _camera, renderDevice));
	return m_renderTargets.back().get();
}

void Moonlit::Renderer::MoonlitRenderer::Render()
{
	for (auto& renderDevice : m_renderDevices)
	{
		for (auto& [mat, drawBuffer] : m_drawBuffers)
			renderDevice->GetOrCreateBatchData(*drawBuffer);
	}
	for (auto& renderTarget : m_renderTargets)
	{
		if (!renderTarget->IsPaused())
			renderTarget->Render();
	}
}

#ifdef RENDER_DEBUG_INFORMATION_QUERY
void Moonlit::Renderer::MoonlitRenderer::InitQueryPool()
{
	vk::QueryPoolCreateInfo createInfo;
	createInfo.sType = vk::StructureType::eQueryPoolCreateInfo;
	createInfo.queryType = vk::QueryType::eTimestamp;
	createInfo.queryCount = 2;
	
	//m_timestampQueryPool = m_device.createQueryPool(createInfo);
}
#endif

void Moonlit::Renderer::MoonlitRenderer::HandleWindowEvents(Moonlit::Inputs::WINDOW_EVENT _event, void* _data)
{
	switch (_event)
	{
	case Moonlit::Inputs::WINDOW_EVENT::WINDOW_RESIZE:
	{
		//Window resize not used by the render target sublayer yet
		//TODO: Add it to the render target sublayer
		//vk::Extent2D* extent = (vk::Extent2D*)_data;
		//m_extent = *extent;
		//m_swapchainOutOfDate = true;
		break;
	}
	case Moonlit::Inputs::WINDOW_EVENT::WINDOW_CLOSE:
	{
		m_windowClosed = true;
		break;
	}
	default:
		//DefWindowProc()
		break;
	}
}

//void Renderer::CreateUboDescriptorSetLayout()
//{
//	vk::DescriptorSetLayoutBinding uboLayoutBinding;
//	uboLayoutBinding.binding = 0;
//	uboLayoutBinding.descriptorType = vk::DescriptorType::eUniformBuffer;
//	uboLayoutBinding.descriptorCount = 1;
//	uboLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eVertex;
//
//	vk::DescriptorSetLayoutCreateInfo createInfo;
//	createInfo.sType = vk::StructureType::eDescriptorSetLayoutCreateInfo;
//	createInfo.bindingCount = 1;
//	createInfo.pBindings = &uboLayoutBinding;
//
//	m_uboDescriptorSetLayout = m_device.createDescriptorSetLayout(createInfo);
//}

//void Renderer::RecordCommandBuffer(vk::CommandBuffer& _buffer, int _imageIndex)
//{
//	vk::CommandBufferBeginInfo beginInfo{};
//	beginInfo.sType = vk::StructureType::eCommandBufferBeginInfo;
//	_buffer.begin(beginInfo);
//
//#ifdef RENDER_DEBUG_INFORMATION_QUERY
//	_buffer.resetQueryPool(m_timestampQueryPool, 0, 2);
//#endif
//
//	vk::ClearValue value;
//	value.color = vk::ClearColorValue();
//	value.color.float32.at(0) = 0.0f;
//	value.color.float32.at(1) = 0.25f;
//	value.color.float32.at(2) = 0.25f;
//	value.color.float32.at(3) = 1.0f;
//
//	vk::ClearValue depthClearValue;
//	depthClearValue.depthStencil = 1.0f;
//
//	std::vector<vk::ClearValue> clearValues = { depthClearValue, value };
//
//	vk::RenderPassBeginInfo renderPassInfo{};
//	renderPassInfo.sType = vk::StructureType::eRenderPassBeginInfo;
//	renderPassInfo.renderPass = m_mainRenderPass;
//	renderPassInfo.framebuffer = m_frameBuffers.at(m_currentFrame);
//	renderPassInfo.renderArea.offset = vk::Offset2D{ 0,0 };
//	renderPassInfo.renderArea.extent = m_extent;
//	renderPassInfo.clearValueCount = 2;
//	renderPassInfo.pClearValues = clearValues.data();
//
//	vk::Viewport viewport;
//	viewport.minDepth = 0.0f;
//	viewport.maxDepth = 1.0f;
//	viewport.width = m_extent.width;
//	viewport.height = m_extent.height;
//	viewport.x = 0;
//	viewport.y = 0;
//
//	_buffer.setViewport(0, viewport);
//
//	vk::Rect2D scissor;
//	scissor.offset = vk::Offset2D{ 0,0 };
//	scissor.extent = m_extent;
//
//	_buffer.setScissor(0, scissor);
//	_buffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
//
//#ifdef RENDER_DEBUG_INFORMATION_QUERY
//	_buffer.writeTimestamp(vk::PipelineStageFlagBits::eTopOfPipe, m_timestampQueryPool, 0);
//#endif
//
//	for (auto& drawBuffer : m_drawBuffers)
//	{
//		drawBuffer.RenderBuffer(_buffer, &m_uboDescriptorSet, 0);
//	}
//
//	_buffer.nextSubpass(vk::SubpassContents::eInline);
//
//	for (auto& drawBuffer : m_drawBuffers)
//	{
//		drawBuffer.RenderBuffer(_buffer, &m_uboDescriptorSet, 1);
//	}
//
//	_buffer.endRenderPass();
//
//#ifdef RENDER_DEBUG_INFORMATION_QUERY
//	
//	_buffer.writeTimestamp(vk::PipelineStageFlagBits::eBottomOfPipe, m_timestampQueryPool, 1);
//#endif
//	
//	_buffer.end();
//}