#include "Renderer/RenderTarget.h"
#include "Renderer/VulkanHelperFunctions.h"

#include "glm/gtc/matrix_transform.hpp"
#include "Renderer/DrawBuffer.h"

RenderTarget::RenderTarget(int _framesInFlight, HWND _surface,
	vk::Instance _instance, Camera* _camera,
	RendererDeviceManager* _deviceManager) : m_instance(_instance), m_camera(_camera)
{
	m_targetWindow = _surface;
	m_framesInFlight = _framesInFlight;
	m_deviceManager = _deviceManager;

	//The device manager need the surface to pick a physical device
	//So we can only link after creating the surface
	CreateSurfaceKHR();
	m_deviceData = _deviceManager->AddTarget(this);

	//Get the format as its gonna be used in the renderpass
	SwapChainSupportDetails swapChainSupport = m_deviceData.SwapChainSupportDetails;
	m_format = vhf::GetFormat(m_deviceData.PhysicalDevice, swapChainSupport.formats);

	Init();
}

void RenderTarget::Init()
{	
	//Create the renderpass before the material as the material needs the renderpass
	CreateRenderPass();
	CreateDescriptorSetLayout();

	m_defaultMaterial = new Material(m_deviceData.Device, 1, m_renderPass, m_uboDescriptorSetLayout);
	
	//Need the format to create the render pass in the renderer
	//TODO: Remove this dependency
	CalculateExtent();

	CreateSwapChainResources();
	CreateCommandPool();
	CreateCommandBuffers();
	CreateUniformBuffers();

	CreateDescriptorPool();
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

	m_capabilities = m_deviceData.PhysicalDevice.getSurfaceCapabilitiesKHR(m_surfaceKHR);

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
	SwapChainSupportDetails swapChainSupport = m_deviceManager->QuerySwapChainSupportDetails(m_surfaceKHR, m_deviceData.PhysicalDevice);

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

	QueueFamilyIndices queueFamilyIndices = m_deviceData.QueueIndices;

	createInfo.imageSharingMode = vk::SharingMode::eExclusive;
	createInfo.queueFamilyIndexCount = 1;
	createInfo.pQueueFamilyIndices = &queueFamilyIndices.graphicsFamily.value();

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
	createInfo.presentMode = swapChainSupport.presentModes[0];
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = nullptr;

	m_swapChain = m_deviceData.Device.createSwapchainKHR(createInfo);
	m_swapChainImages = m_deviceData.Device.getSwapchainImagesKHR(m_swapChain);
}

void RenderTarget::CreateImageViews()
{
	m_swapChainImageViews = new vk::ImageView[m_framesInFlight];
	for (int i = 0; i < m_framesInFlight; ++i)
	{
		m_swapChainImageViews[i] = vhf::CreateImageView(m_deviceData.Device, m_swapChainImages[i], m_format.format, vk::ImageAspectFlagBits::eColor);
	}
}

void RenderTarget::CreateDepthResources()
{
	vk::Format depthFormat = vk::Format::eD32Sfloat;
	uint32_t queueFamilyIndex = m_deviceData.QueueIndices.graphicsFamily.value();

	m_swapChainDepthImages = new vk::Image[m_framesInFlight];
	m_swapChainDepthImageViews = new vk::ImageView[m_framesInFlight];

	for (int i = 0; i < m_framesInFlight; ++i)
	{
		vhf::CreateImage(m_deviceData.Device, m_deviceData.PhysicalDevice, m_extent, depthFormat, vk::ImageTiling::eOptimal,
			vk::ImageUsageFlagBits::eDepthStencilAttachment, vk::MemoryPropertyFlagBits::eDeviceLocal, m_swapChainDepthImages[i], m_swapChainDepthMemory, vk::ImageLayout::eUndefined);
		m_swapChainDepthImageViews[i] = vhf::CreateImageView(m_deviceData.Device, m_swapChainDepthImages[i], depthFormat, vk::ImageAspectFlagBits::eDepth);
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
		m_swapChainFramebuffers[i] = m_deviceData.Device.createFramebuffer(framebufferInfo);
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
	vk::Device device = m_deviceData.Device;
	device.waitIdle();

	device.destroySwapchainKHR(m_swapChain);
	for (int i = 0; i < m_framesInFlight; ++i)
	{
		//m_device.destroyImageView(m_swapChainImageViews[i]);
		//m_device.destroyImage(m_swapChainImages[i]);
		device.destroyImageView(m_swapChainDepthImageViews[i]);
		device.destroyImage(m_swapChainDepthImages[i]);
		device.destroyFramebuffer(m_swapChainFramebuffers[i]);
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
		m_imageAvailableSemaphores[i] = m_deviceData.Device.createSemaphore(semaphoreInfo);
		m_renderFinishedSemaphores[i] = m_deviceData.Device.createSemaphore(semaphoreInfo);
		m_waitForPreviousFrame[i] = m_deviceData.Device.createFence(fenceInfo);
	}
}

void RenderTarget::CreateRenderPass()
{
#pragma region Attachments
	std::vector<vk::AttachmentDescription> attachments;
	attachments.resize(2);
	
	// Depth Attachment
	attachments[0].format = vk::Format::eD32Sfloat;
	attachments[0].samples = vk::SampleCountFlagBits::e1;
	attachments[0].loadOp = vk::AttachmentLoadOp::eClear;
	attachments[0].storeOp = vk::AttachmentStoreOp::eDontCare;
	attachments[0].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	attachments[0].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	attachments[0].initialLayout = vk::ImageLayout::eUndefined;
	attachments[0].finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

	// Color Attachment
	attachments[1].format = m_format.format;
	attachments[1].samples = vk::SampleCountFlagBits::e1;
	attachments[1].loadOp = vk::AttachmentLoadOp::eClear;
	attachments[1].storeOp = vk::AttachmentStoreOp::eStore;
	attachments[1].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	attachments[1].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	attachments[1].initialLayout = vk::ImageLayout::eUndefined;
	attachments[1].finalLayout = vk::ImageLayout::ePresentSrcKHR;

	vk::AttachmentReference* attachmentRefs = new vk::AttachmentReference[2];
	attachmentRefs[0].attachment = 0;
	attachmentRefs[0].layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

	attachmentRefs[1].attachment = 1;
	attachmentRefs[1].layout = vk::ImageLayout::eColorAttachmentOptimal;

#pragma endregion //Attachments

#pragma region Subpasses
	std::vector<vk::SubpassDescription> subpasses;
	std::vector<vk::SubpassDependency> subpassDependencies;
	subpasses.resize(2);
	subpassDependencies.resize(2);

	// Depth Subpass
	subpasses[0].pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
	subpasses[0].colorAttachmentCount = 0;
	subpasses[0].pDepthStencilAttachment = &attachmentRefs[0];

	subpassDependencies[0].srcSubpass = vk::SubpassExternal;
	subpassDependencies[0].dstSubpass = 0;
	subpassDependencies[0].srcStageMask = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
	subpassDependencies[0].dstStageMask = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
	subpassDependencies[0].srcAccessMask = vk::AccessFlagBits::eNone;
	subpassDependencies[0].dstAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentWrite;

	// Color Subpass
	subpasses[1].pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
	subpasses[1].colorAttachmentCount = 1;
	subpasses[1].pColorAttachments = &attachmentRefs[1];
	subpasses[1].pDepthStencilAttachment = &attachmentRefs[0];

	subpassDependencies[1].srcSubpass = 0;
	subpassDependencies[1].dstSubpass = 1;
	subpassDependencies[1].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests;
	subpassDependencies[1].dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests;
	subpassDependencies[1].srcAccessMask = vk::AccessFlagBits::eNone;
	subpassDependencies[1].dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
#pragma endregion //Subpasses

	vk::RenderPassCreateInfo renderPassInfo;
	renderPassInfo.sType = vk::StructureType::eRenderPassCreateInfo;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = static_cast<uint32_t>(subpasses.size());
	renderPassInfo.pSubpasses = subpasses.data();
	renderPassInfo.dependencyCount = static_cast<uint32_t>(subpassDependencies.size());
	renderPassInfo.pDependencies = subpassDependencies.data();

	m_renderPass = m_deviceData.Device.createRenderPass(renderPassInfo);
}

void RenderTarget::CreateCommandPool()
{
	vk::CommandPoolCreateInfo poolInfo;
	poolInfo.sType = vk::StructureType::eCommandPoolCreateInfo;
	poolInfo.queueFamilyIndex = m_deviceData.QueueIndices.graphicsFamily.value();
	poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
	m_commandPool = m_deviceData.Device.createCommandPool(poolInfo);
}

void RenderTarget::CreateCommandBuffers()
{
	m_commandBuffers = new vk::CommandBuffer[m_framesInFlight];

	vk::CommandBufferAllocateInfo allocInfo;
	allocInfo.sType = vk::StructureType::eCommandBufferAllocateInfo;
	allocInfo.commandPool = m_commandPool;
	allocInfo.level = vk::CommandBufferLevel::ePrimary;
	allocInfo.commandBufferCount = m_framesInFlight;
	auto result = m_deviceData.Device.allocateCommandBuffers(allocInfo);
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
		vhf::CreateBuffer(m_deviceData.Device, m_deviceData.PhysicalDevice, bufferInfo);
		m_uniformBuffersMaps[i] = m_deviceData.Device.mapMemory(m_uniformBuffersMemory[i], 0, bufferSize);
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

void RenderTarget::CreateDescriptorSetLayout()
{
	vk::DescriptorSetLayoutBinding uboLayoutBinding;
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = vk::DescriptorType::eUniformBuffer;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eVertex;

	vk::DescriptorSetLayoutCreateInfo layoutInfo;
	layoutInfo.sType = vk::StructureType::eDescriptorSetLayoutCreateInfo;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &uboLayoutBinding;

	m_uboDescriptorSetLayout = m_deviceData.Device.createDescriptorSetLayout(layoutInfo);
}

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

	m_descriptorPool = m_deviceData.Device.createDescriptorPool(poolInfo);
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

	m_descriptorSets = m_deviceData.Device.allocateDescriptorSets(allocInfo);

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

	m_deviceData.Device.updateDescriptorSets(writeSets.size(), writeSets.data(), 0, nullptr);
}

void RenderTarget::Render(std::vector<DrawBuffer> _drawBuffers)
{
	m_deviceData.Device.waitForFences(m_waitForPreviousFrame[m_currentFrame], true, std::numeric_limits<unsigned int>::max());

	m_deviceData.Device.resetFences(m_waitForPreviousFrame[m_currentFrame]);

	UpdateUniformBuffer();

	uint32_t index;
	//Might be an artifact from the old code
	//TODO: Check if it's fine to remove this
	//index = m_device.acquireNextImageKHR(m_swapChain, 
	//	std::numeric_limits<uint64_t>::max(), m_imageAvailableSemaphores[m_currentFrame]).value;

	vk::Semaphore* waitSemaphore = &m_imageAvailableSemaphores[m_currentFrame];
	

	index = m_deviceData.Device.acquireNextImageKHR(m_swapChain, std::numeric_limits<uint64_t>::max(), *waitSemaphore).value;
	
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

	auto result = m_deviceData.Queues.graphicsQueue.submit(1, &submitInfo, m_waitForPreviousFrame[m_currentFrame]);

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
		m_deviceData.Queues.presentQueue.presentKHR(presentInfo);
	}
	catch (vk::OutOfDateKHRError e)
	{
		m_deviceData.Queues.presentQueue.waitIdle();
		m_deviceData.Queues.graphicsQueue.waitIdle();
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
