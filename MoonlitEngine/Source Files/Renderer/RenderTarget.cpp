#include "Renderer/RenderTarget.h"
#include "Renderer/VulkanHelperFunctions.h"

#include "glm/gtc/matrix_transform.hpp"
#include "Renderer/DrawBuffer.h"

RenderTarget::RenderTarget(int _framesInFlight, HWND _surface,
	vk::Instance _instance, Camera* _camera) : m_instance(_instance), m_camera(_camera)
{
	m_targetWindow = _surface;
	m_framesInFlight = _framesInFlight;

	//This is called before Init to allow the DeviceManager to get
	//The present queue
	//TODO: Find a way to allow for device manager to get the present queue without needing a render target
	CreateSurfaceKHR();
}

void RenderTarget::LinkDeviceManager(RendererDeviceManager* _deviceManager)
{
	m_deviceManager = _deviceManager;

	m_device = _deviceManager->GetDevice();
	m_queues = _deviceManager->GetRenderQueues(this);

	SwapChainSupportDetails swapChainSupport = m_deviceManager->QuerySwapChainSupportDetails(m_surfaceKHR, m_deviceManager->GetPhysicalDevice());
	m_format = vhf::GetFormat(m_deviceManager->GetPhysicalDevice(), swapChainSupport.formats);
}

void RenderTarget::Init(vk::DescriptorSetLayout _uboLayout,
	vk::RenderPass _renderPass)
{
	m_renderPass = _renderPass;
	m_uboDescriptorSetLayout = _uboLayout;
	
	//Need the format to create the render pass in the renderer
	//TODO: Remove this dependency
	CalculateExtent();

	CreateSwapChainResources();
	CreateCommandPool();
	CreateCommandBuffers();
	CreateUniformBuffers();

	CreateDescriptorPool();
	//CreateDescriptorSetLayout();
	CreateDescriptorSets();

	CreateSyncObjects();
}

void RenderTarget::SetRenderPass(vk::RenderPass _renderPass)
{
	m_renderPass = _renderPass;
}

void RenderTarget::CalculateExtent()
{
	RECT windowRect;
	GetClientRect(m_targetWindow, &windowRect);

	m_extent.width = static_cast<uint32_t>(windowRect.right - windowRect.left);
	m_extent.height = static_cast<uint32_t>(windowRect.bottom - windowRect.top);

	m_capabilities = m_deviceManager->GetPhysicalDevice().getSurfaceCapabilitiesKHR(m_surfaceKHR);

	if (m_capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		m_extent.width = std::clamp(m_extent.width, m_capabilities.minImageExtent.width, m_capabilities.maxImageExtent.width);
		m_extent.height = std::clamp(m_extent.height, m_capabilities.minImageExtent.height, m_capabilities.maxImageExtent.height);
	}
}

void RenderTarget::CreateSurfaceKHR()
{
	vk::Win32SurfaceCreateInfoKHR createInfo;
	createInfo.sType = vk::StructureType::eWin32SurfaceCreateInfoKHR;
	createInfo.hinstance = GetModuleHandle(nullptr);
	createInfo.hwnd = m_targetWindow;

	m_surfaceKHR = m_instance.createWin32SurfaceKHR(createInfo);
}

void RenderTarget::CreateSwapChainResources()
{
	CreateSwapChain();
	CreateImageViews();
	CreateDepthResources();
	CreateFrameBuffers();
}

void RenderTarget::CreateSwapChain()
{
	SwapChainSupportDetails swapChainSupport = m_deviceManager->QuerySwapChainSupportDetails(m_surfaceKHR, m_deviceManager->GetPhysicalDevice());

	if (m_capabilities.maxImageCount != 0)
	{
		if (m_capabilities.maxImageCount < m_framesInFlight)
		{
			m_framesInFlight = m_capabilities.maxImageCount;
		}
		else if (m_capabilities.minImageCount > m_framesInFlight)
		{
			m_framesInFlight = m_capabilities.minImageCount;
		}
	}

	vk::SwapchainCreateInfoKHR createInfo;
	createInfo.sType = vk::StructureType::eSwapchainCreateInfoKHR;
	createInfo.surface = m_surfaceKHR;
	createInfo.minImageCount = m_framesInFlight;
	createInfo.imageFormat = m_format.format;
	createInfo.imageColorSpace = m_format.colorSpace;
	createInfo.imageExtent = m_extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

	QueueFamilyIndices queueFamilyIndices = m_deviceManager->GetQueueFamilyIndices();

	createInfo.imageSharingMode = vk::SharingMode::eExclusive;
	createInfo.queueFamilyIndexCount = 1;
	createInfo.pQueueFamilyIndices = &queueFamilyIndices.graphicsFamily.value();

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
	createInfo.presentMode = swapChainSupport.presentModes[0];
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = nullptr;

	m_swapChain = m_device.createSwapchainKHR(createInfo);
	m_swapChainImages = m_device.getSwapchainImagesKHR(m_swapChain);
}

void RenderTarget::CreateImageViews()
{
	m_swapChainImageViews = new vk::ImageView[m_framesInFlight];
	for (int i = 0; i < m_framesInFlight; ++i)
	{
		m_swapChainImageViews[i] = vhf::CreateImageView(m_device, m_swapChainImages[i], m_format.format, vk::ImageAspectFlagBits::eColor);
	}
}

void RenderTarget::CreateDepthResources()
{
	vk::Format depthFormat = vk::Format::eD32Sfloat;
	uint32_t queueFamilyIndex = m_deviceManager->GetQueueFamilyIndices().graphicsFamily.value();

	m_swapChainDepthImages = new vk::Image[m_framesInFlight];
	m_swapChainDepthImageViews = new vk::ImageView[m_framesInFlight];

	for (int i = 0; i < m_framesInFlight; ++i)
	{
		vhf::CreateImage(m_device, m_deviceManager->GetPhysicalDevice(), m_extent, depthFormat, vk::ImageTiling::eOptimal,
			vk::ImageUsageFlagBits::eDepthStencilAttachment, vk::MemoryPropertyFlagBits::eDeviceLocal, m_swapChainDepthImages[i], m_swapChainDepthMemory, vk::ImageLayout::eUndefined);
		m_swapChainDepthImageViews[i] = vhf::CreateImageView(m_device, m_swapChainDepthImages[i], depthFormat, vk::ImageAspectFlagBits::eDepth);
	}
}

void RenderTarget::CreateFrameBuffers()
{
	m_swapChainFramebuffers = new vk::Framebuffer[m_framesInFlight];
	for (int i = 0; i < m_framesInFlight; ++i)
	{
		std::vector<vk::ImageView> attachments = {m_swapChainDepthImageViews[i], m_swapChainImageViews[i]};

		vk::FramebufferCreateInfo framebufferInfo;
		framebufferInfo.sType = vk::StructureType::eFramebufferCreateInfo;
		framebufferInfo.renderPass = m_renderPass;
		framebufferInfo.height = m_extent.height;
		framebufferInfo.width = m_extent.width;
		framebufferInfo.layers = 1;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		m_swapChainFramebuffers[i] = m_device.createFramebuffer(framebufferInfo);
	}
}

void RenderTarget::RecreateSwapChain()
{
	DestroySwapChain();

	CalculateExtent();

	CreateSwapChainResources();
}

void RenderTarget::DestroySwapChain()
{
	m_device.waitIdle();

	m_device.destroySwapchainKHR(m_swapChain);
	for (int i = 0; i < m_framesInFlight; ++i)
	{
		//m_device.destroyImageView(m_swapChainImageViews[i]);
		//m_device.destroyImage(m_swapChainImages[i]);
		m_device.destroyImageView(m_swapChainDepthImageViews[i]);
		m_device.destroyImage(m_swapChainDepthImages[i]);
		m_device.destroyFramebuffer(m_swapChainFramebuffers[i]);
	}

	m_swapChainImages.clear();
	delete[] m_swapChainImageViews;
	delete[] m_swapChainDepthImageViews;
	delete[] m_swapChainDepthImages;
	delete[] m_swapChainFramebuffers;
}

void RenderTarget::CreateSyncObjects()
{
	m_imageAvailableSemaphores = new vk::Semaphore[m_framesInFlight];
	m_renderFinishedSemaphores = new vk::Semaphore[m_framesInFlight];
	m_waitForPreviousFrame = new vk::Fence[m_framesInFlight];

	for (size_t i = 0; i < m_framesInFlight; ++i)
	{
		vk::SemaphoreCreateInfo semaphoreInfo;
		semaphoreInfo.sType = vk::StructureType::eSemaphoreCreateInfo;
		vk::FenceCreateInfo fenceInfo;
		fenceInfo.sType = vk::StructureType::eFenceCreateInfo;
		fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;
		m_imageAvailableSemaphores[i] = m_device.createSemaphore(semaphoreInfo);
		m_renderFinishedSemaphores[i] = m_device.createSemaphore(semaphoreInfo);
		m_waitForPreviousFrame[i] = m_device.createFence(fenceInfo);
	}
}

void RenderTarget::CreateCommandPool()
{
	vk::CommandPoolCreateInfo poolInfo;
	poolInfo.sType = vk::StructureType::eCommandPoolCreateInfo;
	poolInfo.queueFamilyIndex = m_deviceManager->GetQueueFamilyIndices().graphicsFamily.value();
	poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
	m_commandPool = m_device.createCommandPool(poolInfo);
}

void RenderTarget::CreateCommandBuffers()
{
	m_commandBuffers = new vk::CommandBuffer[m_framesInFlight];

	vk::CommandBufferAllocateInfo allocInfo;
	allocInfo.sType = vk::StructureType::eCommandBufferAllocateInfo;
	allocInfo.commandPool = m_commandPool;
	allocInfo.level = vk::CommandBufferLevel::ePrimary;
	allocInfo.commandBufferCount = m_framesInFlight;
	auto result = m_device.allocateCommandBuffers(allocInfo);
	vk::CommandBuffer* commandBuffers = result.data();
	for (int i = 0; i < m_framesInFlight; ++i)
	{
		m_commandBuffers[i] = commandBuffers[i];
	}
}

void RenderTarget::CreateUniformBuffers()
{
	uint64_t bufferSize = sizeof(UniformBufferObject);
	m_uniformBuffers = new vk::Buffer[m_framesInFlight];
	m_uniformBuffersMemory = new vk::DeviceMemory[m_framesInFlight];
	m_uniformBuffersMaps = new void* [m_framesInFlight];

	for (int i = 0; i < m_framesInFlight; ++i)
	{
		BufferCreateInfo bufferInfo = {
			.buffer = m_uniformBuffers[i],
			.memory = m_uniformBuffersMemory[i],
			.usage = vk::BufferUsageFlagBits::eUniformBuffer,
			.properties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
			.size = bufferSize
		};
		vhf::CreateBuffer(m_device, m_deviceManager->GetPhysicalDevice(), bufferInfo);
		m_uniformBuffersMaps[i] = m_device.mapMemory(m_uniformBuffersMemory[i], 0, bufferSize);
	}
}

void RenderTarget::UpdateUniformBuffer()
{
	UniformBufferObject ubo;

	ubo.view = m_camera->GetViewMatrix();
	ubo.proj = glm::perspective(glm::radians(90.0f), m_extent.width / (float)m_extent.height, 0.1f, 100.0f);
	ubo.proj[1][1] *= -1; // Vulkan uses a different coordinate system for Y

	memcpy(m_uniformBuffersMaps[m_currentFrame], &ubo, sizeof(UniformBufferObject));
}

//void RenderTarget::CreateDescriptorSetLayout()
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

void RenderTarget::CreateDescriptorPool()
{
	vk::DescriptorPoolSize poolSize;
	poolSize.type = vk::DescriptorType::eUniformBuffer;
	poolSize.descriptorCount = 16;

	vk::DescriptorPoolCreateInfo poolInfo;
	poolInfo.sType = vk::StructureType::eDescriptorPoolCreateInfo;
	poolInfo.maxSets = 16;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;

	m_descriptorPool = m_device.createDescriptorPool(poolInfo);
}

void RenderTarget::CreateDescriptorSets()
{
	std::vector<vk::DescriptorSetLayout> setLayouts;
	for (size_t i = 0; i < m_framesInFlight; ++i)
	{
		setLayouts.push_back(m_uboDescriptorSetLayout);
	}

	vk::DescriptorSetAllocateInfo allocInfo;
	allocInfo.sType = vk::StructureType::eDescriptorSetAllocateInfo;
	allocInfo.descriptorPool = m_descriptorPool;
	allocInfo.descriptorSetCount = m_framesInFlight;
	allocInfo.pSetLayouts = setLayouts.data();

	m_descriptorSets = m_device.allocateDescriptorSets(allocInfo);

	std::vector<vk::WriteDescriptorSet> writeSets;
	std::vector<vk::DescriptorBufferInfo> bufferInfos;
	writeSets.resize(m_framesInFlight);
	bufferInfos.resize(m_framesInFlight);
	for (int i = 0; i < m_framesInFlight; ++i)
	{
		writeSets[i].sType = vk::StructureType::eWriteDescriptorSet;
		writeSets[i].dstSet = m_descriptorSets[i];
		writeSets[i].dstBinding = 0;
		writeSets[i].dstArrayElement = 0;
		writeSets[i].descriptorType = vk::DescriptorType::eUniformBuffer;
		writeSets[i].descriptorCount = 1;

		bufferInfos[i].buffer = m_uniformBuffers[i];
		bufferInfos[i].offset = 0;
		bufferInfos[i].range = sizeof(UniformBufferObject);
		writeSets[i].pBufferInfo = &bufferInfos.data()[i];
	}

	m_device.updateDescriptorSets(writeSets.size(), writeSets.data(), 0, nullptr);
}

void RenderTarget::Render(std::vector<DrawBuffer> _drawBuffers)
{
	m_device.waitForFences(m_waitForPreviousFrame[m_currentFrame], true, std::numeric_limits<unsigned int>::max());

	m_device.resetFences(m_waitForPreviousFrame[m_currentFrame]);

	UpdateUniformBuffer();

	uint32_t index;
	//Might be an artifact from the old code
	//TODO: Check if it's fine to remove this
	//index = m_device.acquireNextImageKHR(m_swapChain, 
	//	std::numeric_limits<uint64_t>::max(), m_imageAvailableSemaphores[m_currentFrame]).value;

	vk::Semaphore* waitSemaphore = &m_imageAvailableSemaphores[m_currentFrame];
	

	index = m_device.acquireNextImageKHR(m_swapChain, std::numeric_limits<uint64_t>::max(), *waitSemaphore).value;
	
	vk::Semaphore* signalSemaphore = &m_renderFinishedSemaphores[index];

	vk::CommandBuffer& buffer = m_commandBuffers[m_currentFrame];
	buffer.reset();
	RecordCommandBuffer(buffer, _drawBuffers);

	
	vk::PipelineStageFlags* waitStages = new vk::PipelineStageFlags(vk::PipelineStageFlagBits::eColorAttachmentOutput);

	vk::SubmitInfo submitInfo;
	submitInfo.sType = vk::StructureType::eSubmitInfo;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphore;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &buffer;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphore;

	auto result = m_queues.graphicsQueue.submit(1, &submitInfo, m_waitForPreviousFrame[m_currentFrame]);

	vk::SwapchainKHR* swapChain = &m_swapChain;

	uint32_t signalIndex = index;

	vk::PresentInfoKHR presentInfo;
	presentInfo.sType = vk::StructureType::ePresentInfoKHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphore;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChain;
	presentInfo.pImageIndices = &index;

	try
	{
		m_queues.presentQueue.presentKHR(presentInfo);
	}
	catch (vk::OutOfDateKHRError e)
	{
		m_queues.presentQueue.waitIdle();
		m_queues.graphicsQueue.waitIdle();
		RecreateSwapChain();
	}

	m_currentFrame = (m_currentFrame + 1) % m_framesInFlight;
}

void RenderTarget::RecordCommandBuffer(vk::CommandBuffer& _buffer, std::vector<DrawBuffer>& _drawBuffers)
{
	vk::CommandBufferBeginInfo beginInfo;
	beginInfo.sType = vk::StructureType::eCommandBufferBeginInfo;
	_buffer.begin(beginInfo);

	vk::ClearValue clearValues[2];
	clearValues[0].depthStencil = vk::ClearDepthStencilValue(1.0f, 1.0f);
	clearValues[1].color = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.25f, 0.25f, 1.0f});

	vk::RenderPassBeginInfo renderPassInfo;
	renderPassInfo.sType = vk::StructureType::eRenderPassBeginInfo;
	renderPassInfo.renderPass = m_renderPass;
	renderPassInfo.framebuffer = m_swapChainFramebuffers[m_currentFrame];
	renderPassInfo.renderArea.offset = vk::Offset2D{ 0, 0 };
	renderPassInfo.renderArea.extent = m_extent;
	renderPassInfo.clearValueCount = 2;
	renderPassInfo.pClearValues = clearValues;

	vk::Viewport viewport;
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = m_extent.width;
	viewport.height = m_extent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	_buffer.setViewport(0, viewport);

	vk::Rect2D scissor;
	scissor.offset = vk::Offset2D{ 0, 0 };
	scissor.extent = m_extent;

	_buffer.setScissor(0, scissor);
	_buffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

	for (auto& drawBuffer : _drawBuffers)
	{
		drawBuffer.RenderBuffer(_buffer, &m_descriptorSets[m_currentFrame], 0);
	}

	_buffer.nextSubpass(vk::SubpassContents::eInline);

	for (auto& drawBuffer : _drawBuffers)
	{
		drawBuffer.RenderBuffer(_buffer, &m_descriptorSets[m_currentFrame], 1);
	}

	_buffer.endRenderPass();

	_buffer.end();
}
