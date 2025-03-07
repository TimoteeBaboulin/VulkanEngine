#include <chrono>
#include <random>

#include "MoonlitVulkan/VulkanRenderer.h"
#include "MoonlitVulkan/VulkanHelperFunctions.h"
#include "MoonlitVulkan/VulkanEngine.h"
#include "glm/gtc/matrix_transform.hpp"
#include "common.h"

#define GLM_FORCE_RADIANS

VulkanRenderer::VulkanRenderer(vk::Extent2D _extent, std::vector<vk::Framebuffer>* _frameBuffers) : m_extent(_extent), m_frameBuffers(_frameBuffers)
{

}

void VulkanRenderer::Init(vk::DescriptorSetLayout _descriptorLayout, vk::ImageView _textureImageView, vk::Sampler _sampler)
{
	InitSyncs();
	CreateCommandBuffers();
	CreateUniformBuffers();
	CreateDescriptorPools();
	CreateDescriptorSets(_descriptorLayout, _textureImageView, _sampler);
}

void VulkanRenderer::Cleanup()
{
	VulkanEngine::LogicalDevice.destroyCommandPool(VulkanEngine::MainCommandPool);

	for (int i = 0; i < m_framesInFlight; i++)
	{
		VulkanEngine::LogicalDevice.destroySemaphore(m_imageAvailable[i]);
		VulkanEngine::LogicalDevice.destroySemaphore(m_renderFinished[i]);
		VulkanEngine::LogicalDevice.destroyFence(m_waitForPreviousFrame[i]);
	}
	
}

void VulkanRenderer::Render(vk::SwapchainKHR _swapchain, RenderInfo _renderInfo, vk::RenderPass _renderPass)
{

	VulkanEngine::LogicalDevice.waitForFences(m_waitForPreviousFrame[m_currentFrame], true, std::numeric_limits<unsigned int>::max());
	VulkanEngine::LogicalDevice.resetFences(m_waitForPreviousFrame[m_currentFrame]);

	UpdateUniformBuffer(m_uniformMaps[m_currentFrame]);

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

//void VulkanRenderer::CreateIndexBuffer()
//{
//	uint64_t bufferSize = sizeof(uint32_t) * m_triangleCount * 3;
//
//	vk::Buffer stagingBuffer;
//	vk::DeviceMemory stagingMemory;
//
//	VertexBufferInfo staging = {
//		.buffer = stagingBuffer,
//		.memory = stagingMemory,
//		.usage = vk::BufferUsageFlagBits::eTransferSrc,
//		.properties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
//		.size = bufferSize
//	};
//
//	CreateBuffer(staging);
//	void* map = VulkanEngine::LogicalDevice.mapMemory(stagingMemory, 0, bufferSize, vk::MemoryMapFlagBits::ePlacedEXT);
//	memcpy(map, m_indexes, bufferSize);
//	VulkanEngine::LogicalDevice.unmapMemory(stagingMemory);
//	map = nullptr;
//
//	VertexBufferInfo vertexBuffer = {
//		.buffer = m_indexBuffer,
//		.memory = m_indexMemory,
//		.usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
//		.properties = vk::MemoryPropertyFlagBits::eDeviceLocal,
//		.size = bufferSize
//	};
//
//	CreateBuffer(vertexBuffer);
//
//	vhf::CopyBufferToBuffer(stagingBuffer, m_indexBuffer, bufferSize);
//
//	VulkanEngine::LogicalDevice.freeMemory(stagingMemory);
//	VulkanEngine::LogicalDevice.destroyBuffer(stagingBuffer);
//}

//void VulkanRenderer::CreateVertexBuffers(vk::Queue _queue)
//{
//	uint64_t bufferSize = sizeof(Vertex) * m_vertexCount;
//
//	vk::Buffer stagingBuffer;
//	vk::DeviceMemory stagingMemory;
//
//	VertexBufferInfo staging = {
//		.buffer = stagingBuffer,
//		.memory = stagingMemory,
//		.usage = vk::BufferUsageFlagBits::eTransferSrc,
//		.properties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
//		.size = bufferSize
//	};
//
//	CreateBuffer(staging);
//	void* map = VulkanEngine::LogicalDevice.mapMemory(stagingMemory, 0, bufferSize, vk::MemoryMapFlagBits::ePlacedEXT);
//	memcpy(map, m_vertices, bufferSize);
//	VulkanEngine::LogicalDevice.unmapMemory(stagingMemory);
//	map = nullptr;
//
//	VertexBufferInfo vertexBuffer = {
//		.buffer = m_vertexBuffer,
//		.memory = m_vertexMemory,
//		.usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
//		.properties = vk::MemoryPropertyFlagBits::eDeviceLocal,
//		.size = bufferSize
//	};
//
//	CreateBuffer(vertexBuffer);
//
//	vhf::CopyBufferToBuffer(stagingBuffer, m_vertexBuffer, bufferSize, _queue);
//
//	VulkanEngine::LogicalDevice.freeMemory(stagingMemory);
//	VulkanEngine::LogicalDevice.destroyBuffer(stagingBuffer);
//}

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
	size.descriptorCount = 1;

	poolInfo.sType = vk::StructureType::eDescriptorPoolCreateInfo;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &size;
	poolInfo.maxSets = 1;

	m_descriptorPools[0] = VulkanEngine::LogicalDevice.createDescriptorPool(poolInfo);
}

void VulkanRenderer::CreateDescriptorSets(vk::DescriptorSetLayout _descriptorLayout, vk::ImageView _textureImageView, vk::Sampler _sampler)
{
	m_descriptorSets.resize(1);
	vk::DescriptorSetAllocateInfo allocInfo;
	allocInfo.sType = vk::StructureType::eDescriptorSetAllocateInfo;
	allocInfo.descriptorPool = m_descriptorPools[0];
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &_descriptorLayout;

	m_descriptorSets = VulkanEngine::LogicalDevice.allocateDescriptorSets(allocInfo);

	std::vector<vk::WriteDescriptorSet> writeSets;

	char i = 0;

	for (auto set : m_descriptorSets)
	{
		vk::DescriptorBufferInfo bufferInfo;
		bufferInfo.buffer = m_uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		vk::DescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		imageInfo.imageView = _textureImageView;
		imageInfo.sampler = _sampler;

		vk::WriteDescriptorSet writeSet;
		writeSet.sType = vk::StructureType::eWriteDescriptorSet;
		writeSet.dstSet = set;
		writeSet.dstBinding = 0;
		writeSet.dstArrayElement = 0;

		writeSet.descriptorCount = 1;
		writeSet.descriptorType = vk::DescriptorType::eUniformBuffer;
		writeSet.pBufferInfo = &bufferInfo;

		vk::WriteDescriptorSet combinedSamplerSet;
		combinedSamplerSet.sType = vk::StructureType::eWriteDescriptorSet;
		combinedSamplerSet.dstSet = set;
		combinedSamplerSet.dstBinding = 1;
		combinedSamplerSet.dstArrayElement = 0;
		combinedSamplerSet.descriptorType = vk::DescriptorType::eCombinedImageSampler;
		combinedSamplerSet.descriptorCount = 1;
		combinedSamplerSet.pImageInfo = &imageInfo;

		writeSets.push_back(writeSet);
		writeSets.push_back(combinedSamplerSet);
		i++;
	}

	VulkanEngine::LogicalDevice.updateDescriptorSets(writeSets.size(), writeSets.data(), 0, nullptr);
}

void VulkanRenderer::UpdateUniformBuffer(void* _map)
{
	UniformBufferObject ubo;

	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(20.0f, 30.0f, 35.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(90.0f), m_extent.width / (float)m_extent.height, 0.1f, 100.0f);
	ubo.proj[1][1] *= -1;

	memcpy(_map, &ubo, sizeof(UniformBufferObject));
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

	vk::Buffer buffers[] = { *_renderInfo.vertexBuffer };
	vk::DeviceSize offsets[] = { 0 };

	_buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, _renderInfo.pipelineLayout, 0, m_descriptorSets[0], nullptr);
	_buffer.bindVertexBuffers(0, buffers, offsets);
	_buffer.bindIndexBuffer(*_renderInfo.indexBuffer, 0, vk::IndexType::eUint32);
	_buffer.drawIndexed(_renderInfo.triangleCount * 3, 1, 0, 0, 0);

	_buffer.nextSubpass(vk::SubpassContents::eInline);
	_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, _renderInfo.pipeline);
	_buffer.drawIndexed(_renderInfo.triangleCount * 3, 1, 0, 0, 0);
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
