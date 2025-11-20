#include "Engine/Renderer/Renderer.h"
#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

#include "Engine/Renderer/RendererDeviceManager.h"
#include "Engine/Renderer/RendererContext.h"
#include "Engine/Renderer/DrawBuffer.h"
#include "common.h"

#include "Engine/Renderer/Material/Material.h"
#include "Camera.h"

#include "ResourceManagement/ResourceManager.h"
#include "Engine/Renderer/RenderTarget.h"

#include "Debug/Logger.h"

MoonlitRenderer::MoonlitRenderer()
{
	m_context = new RendererContext();
}

MoonlitRenderer::~MoonlitRenderer()
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

void MoonlitRenderer::InitContext(ContextInfo& _info, std::vector<const char*> requiredExtensions)
{
	m_context->Init();
	vk::ApplicationInfo appInfo = GetAppInfo(_info.name);
	m_instance = m_context->CreateInstance(appInfo, requiredExtensions.data(), requiredExtensions.Size());
	m_deviceManager = new RendererDeviceManager(*m_instance);
	std::function<void(WINDOW_EVENT, void*)> windowCallback = std::bind(&MoonlitRenderer::HandleWindowEvents, this, std::placeholders::_1, std::placeholders::_2);
}

void MoonlitRenderer::InitVulkan()
{
	//TODO: Repare render profiling
	//m_timeStampPeriods = m_deviceManager->GetTimeStampPeriods();
}

void MoonlitRenderer::InitRenderer()
{

#ifdef RENDER_DEBUG_INFORMATION_QUERY
	InitQueryPool();
#endif

	// TODO: Make the resource manager handle loading the material
	Material* defaultMaterial = new Material("Shaders/BaseMaterial.slang");
	m_drawBuffers.push_back(new DrawBuffer(defaultMaterial));
}

void MoonlitRenderer::Init(ContextInfo& _info, std::vector<const char*> requiredExtensions)
{
	InitContext(_info, requiredExtensions);
	InitVulkan();
	InitRenderer();
}

void MoonlitRenderer::Cleanup()
{
	m_deviceManager->WaitIdleDevices();

	for (auto& renderTarget : m_renderTargets)
	{
		delete renderTarget;
	}

	delete m_deviceManager;
	delete m_context;
}

uint32_t MoonlitRenderer::AddMeshInstance(std::shared_ptr<MeshData> _mesh,
	std::vector<std::shared_ptr<Image>> _textures,
	glm::mat4x4 _model)
{
	// Find the correct draw buffer for this mesh/material
	return m_drawBuffers[0]->AddMeshInstance(_mesh, _textures, _model);
}

void MoonlitRenderer::UpdateInstanceModel(uint32_t _instanceId, glm::mat4x4 _model)
{
	m_drawBuffers[0]->UpdateInstanceModel(_instanceId, _model);
}

void MoonlitRenderer::RemoveMeshInstance(uint32_t _instanceId)
{
	m_drawBuffers[0]->RemoveMeshInstance(_instanceId);
}

RenderTarget* MoonlitRenderer::AddRenderTarget(void* _handle, Camera* _camera)
{
	m_renderTargets.push_back(new RenderTarget(3, (HWND)_handle, *m_instance, _camera, m_deviceManager));
	for (auto it = m_drawBuffers.begin(); it != m_drawBuffers.end(); it++)
	{
		(*it)->LinkTarget(*m_renderTargets.back());
	}
	return m_renderTargets.back();
}

void MoonlitRenderer::LoadMesh(std::string name)
{
	////TODO: Don't load the mesh in the renderer, it should be done by the engine itself through gameobjects
	//std::shared_ptr<MeshData> testMesh;
	//if (!ResourceManager::TryGetResource<MeshData>(name, testMesh))
	//{
	//	std::string errorMsg = "Mesh " + name + " not found in ResourceManager.";
	//	LOG_ERROR(errorMsg.c_str());
	//	throw std::runtime_error(errorMsg);
	//}

	//glm::mat4x4 scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
	//glm::mat4x4 rotate = glm::rotate(0.0f, glm::vec3(0, 1, 0));
	//glm::mat4x4 translate = glm::translate(glm::vec3(0, 0, 0));

	//glm::mat4x4 model = translate * rotate * scale;

	//std::shared_ptr<Image> firstTexture;
	//std::vector<std::shared_ptr<Image>> m_textures;
	//if (ResourceManager::TryGetResource<Image>("barstool_albedo", firstTexture))
	//{
	//	m_textures.push_back(firstTexture);
	//}
	//else
	//{
	//	LOG_ERROR("Failed to load texture barstool_albedo.png from ResourceManager.");
	//}

	//std::shared_ptr<Image> secondTexture;
	//std::vector<std::shared_ptr<Image>> secondaryTextures;
	//if (ResourceManager::TryGetResource<Image>("sniper_albedo", secondTexture))
	//{
	//	secondaryTextures.push_back(secondTexture);
	//}
	//else
	//{
	//	LOG_ERROR("Failed to load texture sniper_albedo.png from ResourceManager.");
	//}

	//if (!test)
	//{
	//	for (int i = -5; i < 6; i++)
	//	{
	//		glm::vec3 vec = glm::vec3(i, -i, i);
	//		translate = glm::translate(glm::vec3(i, -i, i));

	//		glm::mat4x4 model = translate * rotate * scale;
	//		//MeshInstance* instance = new MeshInstance{ testMesh.get(), m_textures, model};

	//		//m_drawBuffers[0]->TryAddMesh(instance);
	//	}
	//	
	//}
	//else
	//{
	//	for (int i = -5; i < 6; i++)
	//	{
	//		translate = glm::translate(glm::vec3(i, 0, 0));

	//		glm::mat4x4 model = translate * rotate * scale;
	//		//MeshInstance* instance = new MeshInstance{ testMesh.get(), m_textures, model};

	//		//m_drawBuffers[0]->TryAddMesh(instance);
	//	}
	//}
	//test = !test;

	//m_drawBuffers[0]->UpdateBuffers();
}

void MoonlitRenderer::Render()
{
	//std::cout << "Rendering frame " << m_currentFrame << std::endl;
	for (auto& renderTarget : m_renderTargets)
	{
		if (!renderTarget->IsPaused())
			renderTarget->Render(m_drawBuffers);
	}

	//std::cout << "Frame rendered" << std::endl;

// Those have not been added to the render target layer yet
// TODO: Add them to the render target layer
//	if (m_windowClosed)
//	{
//		return;
//	}
//
//#ifdef RENDER_DEBUG_INFORMATION_QUERY
//	uint64_t* timestamps = new uint64_t[2];
//	m_device.getQueryPoolResults(
//		m_timestampQueryPool, 
//		0, 
//		2, 
//		2 * sizeof(uint64_t), 
//		timestamps, 
//		sizeof(uint64_t), 
//		vk::QueryResultFlagBits::e64 | vk::QueryResultFlagBits::eWait);
//
//	float delta_in_ms = float(timestamps[1] - timestamps[0]) * m_timeStampPeriods / 1000000.0f;
//	std::cout << "Rendering " << m_drawBuffers[0].GetTriangleCount() << " triangles" << std::endl;
//	std::cout << "Frame time: " << delta_in_ms << std::endl;
//#endif
//
//	try
//	{
//		m_graphicsQueue.presentKHR(presentInfo);
//	}
//	catch (vk::OutOfDateKHRError err)
//	{
//		m_swapchainOutOfDate = true;
//	}
}

#ifdef RENDER_DEBUG_INFORMATION_QUERY
void MoonlitRenderer::InitQueryPool()
{
	vk::QueryPoolCreateInfo createInfo;
	createInfo.sType = vk::StructureType::eQueryPoolCreateInfo;
	createInfo.queryType = vk::QueryType::eTimestamp;
	createInfo.queryCount = 2;
	
	//m_timestampQueryPool = m_device.createQueryPool(createInfo);
}
#endif

//void Renderer::CreateRenderPasses(vk::Format _format)
//{
//#pragma region Color
//	vk::AttachmentDescription colorAttachment;
//	colorAttachment.format = _format;
//	colorAttachment.samples = vk::SampleCountFlagBits::e1;
//	colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
//	colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
//	colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
//	colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
//	colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
//	colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;
//
//	vk::AttachmentReference colorRef{};
//	colorRef.attachment = 1;
//	colorRef.layout = vk::ImageLayout::ePresentSrcKHR;
//#pragma endregion
//
//#pragma region Depth
//	colorRef.layout = vk::ImageLayout::eColorAttachmentOptimal;
//
//	vk::AttachmentDescription depthAttachment;
//	depthAttachment.format = vk::Format::eD32Sfloat;
//	depthAttachment.samples = vk::SampleCountFlagBits::e1;
//	depthAttachment.loadOp = vk::AttachmentLoadOp::eClear;
//	depthAttachment.storeOp = vk::AttachmentStoreOp::eStore;
//	depthAttachment.stencilLoadOp = vk::AttachmentLoadOp::eClear;
//	depthAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
//	depthAttachment.initialLayout = vk::ImageLayout::eUndefined;
//	depthAttachment.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
//
//	vk::AttachmentReference depthRef;
//	depthRef.attachment = 0;
//	depthRef.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
//#pragma endregion
//
//#pragma region Depth Subpass
//	vk::SubpassDescription depthSubpass;
//	depthSubpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
//	depthSubpass.colorAttachmentCount = 0;
//	depthSubpass.pDepthStencilAttachment = &depthRef;
//
//	vk::SubpassDependency depthSubDep;
//	depthSubDep.srcSubpass = vk::SubpassExternal;
//	depthSubDep.dstSubpass = 0;
//
//	depthSubDep.srcStageMask = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
//	depthSubDep.dstStageMask = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
//
//	depthSubDep.srcAccessMask = vk::AccessFlagBits::eNone;
//	depthSubDep.dstAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentWrite;
//#pragma endregion
//
//#pragma region MainSubpass
//	vk::SubpassDescription colorSubpass{};
//	colorSubpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
//	colorSubpass.colorAttachmentCount = 1;
//	colorSubpass.pColorAttachments = &colorRef;
//	colorSubpass.pDepthStencilAttachment = &depthRef;
//
//	vk::SubpassDependency dependency;
//	dependency.srcSubpass = 0;
//	dependency.dstSubpass = 1;
//
//	dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests;
//	dependency.srcAccessMask = vk::AccessFlagBits::eNone;
//	dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests;
//	dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
//#pragma endregion
//
//	std::vector <vk::AttachmentDescription> attachments = { depthAttachment, colorAttachment };
//	std::vector <vk::SubpassDescription> subpasses = { depthSubpass, colorSubpass };
//	std::vector <vk::SubpassDependency> dependencies = { depthSubDep, dependency };
//
//	vk::RenderPassCreateInfo renderpass{};
//	renderpass.sType = vk::StructureType::eRenderPassCreateInfo;
//	renderpass.attachmentCount = attachments.size();
//	renderpass.pAttachments = attachments.data();
//	renderpass.subpassCount = 2;
//	renderpass.pSubpasses = subpasses.data();
//	renderpass.dependencyCount = 2;
//	renderpass.pDependencies = dependencies.data();
//
//	m_mainRenderPass = m_device.createRenderPass(renderpass);
//}

void MoonlitRenderer::HandleWindowEvents(WINDOW_EVENT _event, void* _data)
{
	switch (_event)
	{
	case WINDOW_EVENT::WINDOW_RESIZE:
	{
		//Window resize not used by the render target sublayer yet
		//TODO: Add it to the render target sublayer
		//vk::Extent2D* extent = (vk::Extent2D*)_data;
		//m_extent = *extent;
		//m_swapchainOutOfDate = true;
		break;
	}
	case WINDOW_EVENT::WINDOW_CLOSE:
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