#include <chrono>
#include <random>
#include <Windows.h>
#include <Xinput.h>

#include "MoonlitVulkan/VulkanRenderer.h"
#include "MoonlitVulkan/VulkanHelperFunctions.h"
#include "MoonlitVulkan/VulkanEngine.h"
#include "glm/gtc/matrix_transform.hpp"
#include "common.h"

#include "MoonlitVulkan/VulkanData.h"
#include "Camera.h"

#define GLM_FORCE_RADIANS

VulkanRenderer::VulkanRenderer(vk::Extent2D _extent, std::vector<vk::Framebuffer>* _frameBuffers) : m_extent(_extent), m_frameBuffers(_frameBuffers)
{
	m_cameras.push_back(new Camera(glm::vec3(20.0f, 30.0f, 35.0f), glm::vec3(1.0, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

	m_inputHandler = new CameraInputHandler(m_cameras[0]);
	InputManager::GetInstance()->AddInputHandler(m_inputHandler);
	InputManager::GetInstance()->LockCursor();
}

void VulkanRenderer::Init(vk::DescriptorSetLayout _uboDescriptorLayout, vk::DescriptorSetLayout _shaderDescriptorLayout)
{
	InitSyncs();
	CreateCommandBuffers();
	CreateUniformBuffers();
	CreateDescriptorPools();
	CreateDescriptorSets(_uboDescriptorLayout);
	m_shaderDescriptorLayout = _shaderDescriptorLayout;
}

void VulkanRenderer::Cleanup()
{
	vk::Device& device = VulkanEngine::LogicalDevice;

	for (int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i].CleanUp(device);
	}

	device.freeCommandBuffers(VulkanEngine::MainCommandPool, m_commandBuffers);
	device.destroyCommandPool(VulkanEngine::MainCommandPool);

	for (int i = 0; i < m_uniformBuffers.size(); i++)
	{
		device.unmapMemory(m_uniformMemories[i]);
		device.destroyBuffer(m_uniformBuffers[i]);
	}

	for (int i = 0; i < m_descriptorSets.size(); i++)
	{
		device.freeDescriptorSets(m_descriptorPools[i], (uint32_t)m_descriptorSets.size(), m_descriptorSets.data());
	}

	for (int i = 0; i < m_descriptorPools.size(); i++)
	{
		device.destroyDescriptorPool(m_descriptorPools[i]);
	}

	for (int i = 0; i < m_framesInFlight; i++)
	{
		device.destroySemaphore(m_imageAvailable[i]);
		device.destroySemaphore(m_renderFinished[i]);
		device.destroyFence(m_waitForPreviousFrame[i]);
	}
	
	device.freeDescriptorSets(m_descriptorPools[0], m_descriptorSets);	
}

void VulkanRenderer::LoadMesh(MeshData& _mesh)
{
	Mesh mesh;
	mesh.Load(VulkanEngine::LogicalDevice, _mesh, m_shaderDescriptorLayout, m_descriptorPools[0]);
	m_meshes.push_back(mesh);
}

void VulkanRenderer::Render(vk::SwapchainKHR _swapchain, RenderInfo _renderInfo, vk::RenderPass _renderPass)
{

	VulkanEngine::LogicalDevice.waitForFences(m_waitForPreviousFrame[m_currentFrame], true, std::numeric_limits<unsigned int>::max());
	VulkanEngine::LogicalDevice.resetFences(m_waitForPreviousFrame[m_currentFrame]);

	UpdateUniformBuffer(m_uniformMaps[m_currentFrame], m_cameras[0]);

	uint32_t index;

	index = VulkanEngine::LogicalDevice.acquireNextImageKHR(_swapchain, std::numeric_limits<unsigned int>::max(), m_imageAvailable[m_currentFrame]).value;
	vk::CommandBuffer& buffer = m_commandBuffers[m_currentFrame];
	buffer.reset();
	RecordCommandBuffer(buffer, index, _renderInfo, _renderPass);

	vk::Semaphore waitSemaphores[] = { m_imageAvailable[m_currentFrame]};
	vk::Semaphore signalSemaphores[] = { m_renderFinished[m_currentFrame]};
	vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
	vk::SubmitInfo submitInfo;
	submitInfo.sType = vk::StructureType::eSubmitInfo;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.pCommandBuffers = &buffer;
	submitInfo.commandBufferCount = 1;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	VulkanEngine::GraphicsQueue.submit(submitInfo, m_waitForPreviousFrame[m_currentFrame]);

	vk::SwapchainKHR swapChains[] = { _swapchain };

	vk::PresentInfoKHR presentInfo;
	presentInfo.sType = vk::StructureType::ePresentInfoKHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &index;
	VulkanEngine::GraphicsQueue.presentKHR(presentInfo);

	m_currentFrame = (m_currentFrame + 1) % m_framesInFlight;
}

void VulkanRenderer::InitSyncs()
{
	m_imageAvailable.resize(m_framesInFlight);
	m_renderFinished.resize(m_framesInFlight);
	m_waitForPreviousFrame.resize(m_framesInFlight);

	vk::SemaphoreCreateInfo semaphoreInfo;
	semaphoreInfo.sType = vk::StructureType::eSemaphoreCreateInfo;

	vk::FenceCreateInfo fenceInfo;
	fenceInfo.sType = vk::StructureType::eFenceCreateInfo;
	fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;

	for (int i = 0; i < m_framesInFlight; i++)
	{
		m_imageAvailable[i] = VulkanEngine::LogicalDevice.createSemaphore(semaphoreInfo);
		m_renderFinished[i] = VulkanEngine::LogicalDevice.createSemaphore(semaphoreInfo);
		m_waitForPreviousFrame[i] = VulkanEngine::LogicalDevice.createFence(fenceInfo);
	}
}

void VulkanRenderer::CreateUniformBuffers()
{
	uint64_t bufferSize = sizeof(UniformBufferObject);

	m_uniformBuffers.resize(m_framesInFlight);
	m_uniformMemories.resize(m_framesInFlight);
	m_uniformMaps.resize(m_framesInFlight);

	for (int i = 0; i < m_framesInFlight; i++)
	{
		VertexBufferInfo info = {
		.buffer = m_uniformBuffers[i],
		.memory = m_uniformMemories[i],
		.usage = vk::BufferUsageFlagBits::eUniformBuffer,
		.properties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
		.size = bufferSize
		};

		vhf::CreateBuffer(info);
		m_uniformMaps[i] = VulkanEngine::LogicalDevice.mapMemory(m_uniformMemories[i], 0, bufferSize);
	}
	
}

void VulkanRenderer::CreateDescriptorPools()
{
	m_descriptorPools.resize(1);
	vk::DescriptorPoolSize size;
	vk::DescriptorPoolCreateInfo poolInfo;
	size.type = vk::DescriptorType::eUniformBuffer;
	size.descriptorCount = 10;

	poolInfo.sType = vk::StructureType::eDescriptorPoolCreateInfo;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &size;
	poolInfo.maxSets = 16;

	m_descriptorPools[0] = VulkanEngine::LogicalDevice.createDescriptorPool(poolInfo);
}

void VulkanRenderer::CreateDescriptorSets(vk::DescriptorSetLayout _descriptorLayout)
{
	m_descriptorSets.resize(1);
	vk::DescriptorSetAllocateInfo allocInfo;
	allocInfo.sType = vk::StructureType::eDescriptorSetAllocateInfo;
	allocInfo.descriptorPool = m_descriptorPools[0];
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &_descriptorLayout;

	m_descriptorSets = VulkanEngine::LogicalDevice.allocateDescriptorSets(allocInfo);

	std::vector<vk::WriteDescriptorSet> writeSets;

	vk::DescriptorBufferInfo bufferInfo;
	bufferInfo.buffer = m_uniformBuffers[0];
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(UniformBufferObject);

	vk::WriteDescriptorSet writeSet;
	writeSet.sType = vk::StructureType::eWriteDescriptorSet;
	writeSet.dstSet = m_descriptorSets[0];
	writeSet.dstBinding = 0;
	writeSet.dstArrayElement = 0;
	writeSet.descriptorCount = 1;
	writeSet.descriptorType = vk::DescriptorType::eUniformBuffer;
	writeSet.pBufferInfo = &bufferInfo;

	writeSets.push_back(writeSet);

	VulkanEngine::LogicalDevice.updateDescriptorSets(writeSets.size(), writeSets.data(), 0, nullptr);
}

void VulkanRenderer::UpdateUniformBuffer(void* _map, Camera* _camera)
{
	UniformBufferObject ubo;

	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	ubo.model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = _camera->GetViewMatrix();
	ubo.proj = glm::perspective(glm::radians(90.0f), m_extent.width / (float)m_extent.height, 0.1f, 100.0f);
	ubo.proj[1][1] *= -1;

	memcpy(_map, &ubo, sizeof(UniformBufferObject));
}

void VulkanRenderer::BindDescriptorSets(vk::PipelineLayout& _layout, Mesh& _mesh, vk::CommandBuffer& _cmdBuffer)
{
	
}


void VulkanRenderer::RecordCommandBuffer(vk::CommandBuffer& _buffer, int _imageIndex, RenderInfo& _renderInfo, vk::RenderPass _renderPass)
{
	vk::CommandBufferBeginInfo beginInfo{};
	beginInfo.sType = vk::StructureType::eCommandBufferBeginInfo;
	_buffer.begin(beginInfo);

	vk::ClearValue value;
	value.color = vk::ClearColorValue();
	value.color.float32.at(0) = 0.0f ;
	value.color.float32.at(1) = 0.25f;
	value.color.float32.at(2) = 0.25f;
	value.color.float32.at(3) = 1.0f ;

	vk::ClearValue depthClearValue;
	depthClearValue.depthStencil = 1.0f;

	std::vector<vk::ClearValue> clearValues = { depthClearValue, value };

	vk::RenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = vk::StructureType::eRenderPassBeginInfo;
	renderPassInfo.renderPass = _renderPass;
	renderPassInfo.framebuffer = m_frameBuffers->at(m_currentFrame);
	renderPassInfo.renderArea.offset = vk::Offset2D{ 0,0 };
	renderPassInfo.renderArea.extent = m_extent;
	renderPassInfo.clearValueCount = 2;
	renderPassInfo.pClearValues = clearValues.data();

	_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, _renderInfo.depthPipeline);
	_buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, _renderInfo.pipelineLayout, 0, 1, &m_descriptorSets[0], 0, nullptr);

	_buffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

	vk::Viewport viewport;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	viewport.width = m_extent.width;
	viewport.height = m_extent.height;
	viewport.x = 0;
	viewport.y = 0;

	_buffer.setViewport(0, viewport);

	vk::Rect2D scissor;
	scissor.offset = vk::Offset2D{ 0,0 };
	scissor.extent = m_extent;
	_buffer.setScissor(0, scissor);

	for(auto& Mesh : m_meshes)
	{
		vk::DeviceSize offsets[] = { 0 };

		Mesh.BindSets(_buffer, _renderInfo.pipelineLayout);
		_buffer.bindVertexBuffers(0, Mesh.m_vertexBuffer, offsets);
		_buffer.bindIndexBuffer(Mesh.m_indexBuffer, 0, vk::IndexType::eUint32);
		_buffer.drawIndexed(Mesh.m_triangleCount * 3, 1, 0, 0, 0);
	}

	_buffer.nextSubpass(vk::SubpassContents::eInline);
	_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, _renderInfo.pipeline);

	for (auto& Mesh : m_meshes)
	{
		vk::DeviceSize offsets[] = { 0 };

		BindDescriptorSets(_renderInfo.pipelineLayout, Mesh, _buffer);

		_buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, _renderInfo.pipelineLayout, 0, m_descriptorSets[0], nullptr);
		_buffer.bindVertexBuffers(0, Mesh.m_vertexBuffer, offsets);
		_buffer.bindIndexBuffer(Mesh.m_indexBuffer, 0, vk::IndexType::eUint32);
		_buffer.drawIndexed(Mesh.m_triangleCount * 3, 1, 0, 0, 0);
	}

	_buffer.endRenderPass();
	_buffer.end();
}

void VulkanRenderer::CreateCommandBuffers()
{
	m_commandBuffers.reserve(m_framesInFlight);

	vk::CommandBufferAllocateInfo bufferInfo{};
	bufferInfo.sType = vk::StructureType::eCommandBufferAllocateInfo;
	bufferInfo.commandPool = VulkanEngine::MainCommandPool;
	bufferInfo.commandBufferCount = m_framesInFlight;
	bufferInfo.level = vk::CommandBufferLevel::ePrimary;

	m_commandBuffers = VulkanEngine::LogicalDevice.allocateCommandBuffers(bufferInfo);
}

void CameraInputHandler::HandleMouseMoveInput(int _deltaX, int _deltaY)
{
	m_camera->Rotate(glm::vec3(0.0f, -1.0f, 0.0f), _deltaX * 0.01f);
	m_camera->Rotate(glm::vec3(-1.0f, 0.0f, 0.0f), _deltaY * 0.01f);
}

void CameraInputHandler::HandleMouseInput(MOUSE_KEY _key, bool _keyDown)
{
}

void CameraInputHandler::HandleKeyboardInput(KEYBOARD_KEY _key, bool _keyDown)
{
	switch (_key)
	{
	case KEYBOARD_KEY::ARROW_LEFT:
		m_camera->Translate(-m_camera->GetRightVector());
		break;
	case KEYBOARD_KEY::ESCAPE:
		InputManager::GetInstance()->UnlockCursor();
		break;
	case KEYBOARD_KEY::SPACE:
		InputManager::GetInstance()->LockCursor();
		break;
	}
}

void CameraInputHandler::HandleGamepadInput(GAMEPAD_KEY _key, bool _keyDown)
{
	

	switch (_key)
	{
	case GAMEPAD_KEY::DPAD_LEFT:
		m_camera->Translate(-m_camera->GetRightVector());
		break;
	case GAMEPAD_KEY::DPAD_RIGHT:
		m_camera->Translate(m_camera->GetRightVector());
		break;
	default:
		break;
	}
}

void CameraInputHandler::HandleGamepadAxis(GAMEPAD_KEY _key, float _x, float _y)
{
	switch (_key)
	{
	case AXIS_RIGHT:
		m_camera->Rotate(glm::vec3(0.0f, -1.0f, 0.0f), _x * 0.1f);
		m_camera->Rotate(glm::vec3(1.0f, 0.0f, 0.0f), _y * 0.1f);
		break;
	case AXIS_LEFT:
	{
		glm::vec3 translation = m_camera->GetForwardVector() * _y + m_camera->GetRightVector() * _x;
		m_camera->Translate(translation);
		break;
	}
	default:
		break;
	}
}
