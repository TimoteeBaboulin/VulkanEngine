#include "VulkanPipeline.h"
#include "VulkanEngine.h"
#include "VulkanHelperFunctions.h"

VulkanPipeline::VulkanPipeline(VulkanPipelineInfo _info) : m_info(_info)
{
	m_vertexCount = 0;
	m_triangleCount = 0;
}

void VulkanPipeline::Init(vk::Extent2D _extent)
{
	CreateVertexBuffer();
	CreateIndexBuffer();

	CreateSwapChain(_extent);
	CreateImageViews();
	CreateRenderPasses();
	CreateDescriptorSetLayout();
	CreateRenderPipeline();

	CreateFrameBuffers();
}

void VulkanPipeline::Init(vk::Extent2D _extent, aiMesh* _mesh)
{
	Load(_mesh);
	Init(_extent);
}

void VulkanPipeline::Cleanup()
{
	for (auto imageView : m_imageViews)
	{
		VulkanEngine::LogicalDevice.destroyImageView(imageView);
	}

	VulkanEngine::LogicalDevice.destroyPipeline(m_pipeline);
	VulkanEngine::LogicalDevice.destroyPipelineLayout(m_pipelineLayout);
	VulkanEngine::LogicalDevice.destroyRenderPass(m_renderPass);
}

RenderInfo VulkanPipeline::GetRenderInfo()
{
	RenderInfo info = {
		.renderPass = m_renderPass,
		.pipeline = m_pipeline,
		.pipelineLayout = m_pipelineLayout,
		.vertexBuffer = &m_vertexBuffer,
		.indexBuffer = &m_indexBuffer,
		.vertexCount = m_vertexCount,
		.triangleCount = m_triangleCount
	};

	return info;
}

void VulkanPipeline::Load(aiMesh* _mesh)
{
	AddMesh(_mesh);

	if (m_vertexBuffer != nullptr)
	{
		VulkanEngine::LogicalDevice.destroyBuffer(m_vertexBuffer);
		VulkanEngine::LogicalDevice.destroyBuffer(m_indexBuffer);
	}

	CreateVertexBuffer();
	CreateIndexBuffer();
}

vk::PresentModeKHR VulkanPipeline::GetPresentMode(std::vector<vk::PresentModeKHR>& _modes)
{
	if (std::find(_modes.begin(), _modes.end(), vk::PresentModeKHR::eMailbox) != _modes.end())
		return vk::PresentModeKHR::eMailbox;

	return vk::PresentModeKHR::eFifo;
}

void VulkanPipeline::CreateDescriptorSetLayout()
{
	vk::DescriptorSetLayoutBinding uboLayoutBinding;
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = vk::DescriptorType::eUniformBuffer;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eVertex;

	vk::DescriptorSetLayoutCreateInfo createInfo;
	createInfo.sType = vk::StructureType::eDescriptorSetLayoutCreateInfo;
	createInfo.bindingCount = 1;
	createInfo.pBindings = &uboLayoutBinding;
	
	m_descriptorLayout = VulkanEngine::LogicalDevice.createDescriptorSetLayout(createInfo);
}

void VulkanPipeline::CreateSwapChain(vk::Extent2D _extent)
{
	vk::SurfaceFormatKHR format = GetFormat(m_info.details.formats);
	vk::Extent2D extent = _extent;
	vk::PresentModeKHR presentMode = GetPresentMode(m_info.details.presentModes);

	unsigned int frameBufferCount;
	if (m_info.details.capabilities.maxImageCount == 0 || m_info.details.capabilities.maxImageCount >= m_info.details.capabilities.minImageCount + 1)
		frameBufferCount = m_info.details.capabilities.minImageCount + 1;
	else
		frameBufferCount = m_info.details.capabilities.minImageCount;

	vk::SwapchainCreateInfoKHR createInfo{};
	createInfo.sType = vk::StructureType::eSwapchainCreateInfoKHR;
	createInfo.setSurface(m_info.surface);

	createInfo.minImageCount = frameBufferCount;
	createInfo.imageFormat = format.format;
	createInfo.imageColorSpace = format.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

	uint32_t queueFamilyIndices[] = { VulkanEngine::FamilyIndices.graphicsFamily.value(), VulkanEngine::FamilyIndices.khrPresentFamily.value() };

	if (VulkanEngine::FamilyIndices.graphicsFamily != VulkanEngine::FamilyIndices.khrPresentFamily) {
		createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = vk::SharingMode::eExclusive;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	createInfo.preTransform = m_info.details.capabilities.currentTransform;
	createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = nullptr;

	m_extent = extent;
	m_format = format.format;
	m_swapChain = VulkanEngine::LogicalDevice.createSwapchainKHR(createInfo);
	m_images = VulkanEngine::LogicalDevice.getSwapchainImagesKHR(m_swapChain);
}

void VulkanPipeline::CreateImageViews()
{
	m_imageViews.resize(m_images.size());

	int i = 0;

	for (auto image : m_images)
	{
		m_imageViews[i] = vhf::CreateImageView(image, m_format.format, vk::ImageAspectFlagBits::eColor);
		i++;
	}
}

void VulkanPipeline::CreateDepthImage()
{
	vk::Format format = vk::Format::eD32Sfloat;

	vhf::CreateImage(m_extent, format, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eDepthStencilAttachment, vk::MemoryPropertyFlagBits::eDeviceLocal, m_depthImage, m_depthMemory, vk::ImageLayout::eDepthAttachmentOptimal);
	m_depthImageView = vhf::CreateImageView(m_depthImage, format, vk::ImageAspectFlagBits::eDepth);
}

void VulkanPipeline::CreateRenderPasses()
{
	vk::AttachmentDescription colorAttachment;
	colorAttachment.format = m_format.format;
	colorAttachment.samples = vk::SampleCountFlagBits::e1;
	colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
	colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
	colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
	colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

	vk::AttachmentReference colorRef{};
	colorRef.attachment = 0;
	colorRef.layout = vk::ImageLayout::eColorAttachmentOptimal;


	//TODO: Finish implementing Depth Buffer
	vk::AttachmentDescription depthAttachment;
	depthAttachment.format = m_format.format;
	depthAttachment.samples = vk::SampleCountFlagBits::e1;
	depthAttachment.loadOp = vk::AttachmentLoadOp::eClear;
	depthAttachment.storeOp = vk::AttachmentStoreOp::eDontCare;
	depthAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	depthAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	depthAttachment.initialLayout = vk::ImageLayout::eUndefined;
	depthAttachment.finalLayout = vk::ImageLayout::eDepthAttachmentOptimal;

	vk::AttachmentReference depthRef;
	depthRef.attachment = 1;
	depthRef.layout = vk::ImageLayout::eDepthAttachmentOptimal;
	//Endof Depth Buffer Ref

	vk::SubpassDescription subpass{};
	subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorRef;
	subpass.pDepthStencilAttachment = nullptr;

	vk::SubpassDependency dependency;
	dependency.srcSubpass = vk::SubpassExternal;
	dependency.dstSubpass = 0;

	dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests;
	dependency.srcAccessMask = vk::AccessFlagBits::eNone;
	dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests;
	dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite;

	std::vector <vk::AttachmentDescription> attachments = { colorAttachment};

	vk::RenderPassCreateInfo renderpass{};
	renderpass.sType = vk::StructureType::eRenderPassCreateInfo;
	renderpass.attachmentCount = attachments.size();
	renderpass.pAttachments = attachments.data();
	renderpass.subpassCount = 1;
	renderpass.pSubpasses = &subpass;
	renderpass.dependencyCount = 1;
	renderpass.pDependencies = &dependency;

	m_renderPass = VulkanEngine::LogicalDevice.createRenderPass(renderpass);
}

void VulkanPipeline::CreateRenderPipeline()
{
	//Shader Information
	auto vertexShaderBytes = readFile("Shaders/vert.spv");
	auto fragmentShaderBytes = readFile("Shaders/frag.spv");
	vk::ShaderModule vertexModule = WrapShader(vertexShaderBytes);
	vk::ShaderModule fragmentModule = WrapShader(fragmentShaderBytes);

	vk::PipelineShaderStageCreateInfo vertexInfo{};
	vertexInfo.sType = vk::StructureType::ePipelineShaderStageCreateInfo;
	vertexInfo.stage = vk::ShaderStageFlagBits::eVertex;
	vertexInfo.module = vertexModule;
	vertexInfo.pName = "main";

	vk::PipelineShaderStageCreateInfo fragmentInfo{};
	fragmentInfo.sType = vk::StructureType::ePipelineShaderStageCreateInfo;
	fragmentInfo.stage = vk::ShaderStageFlagBits::eFragment;
	fragmentInfo.module = fragmentModule;
	fragmentInfo.pName = "main";

	vk::PipelineShaderStageCreateInfo createInfos[] = { vertexInfo, fragmentInfo };

	//Vertex Information
	vk::VertexInputBindingDescription vBinding;
	vBinding.stride = 24;
	vBinding.inputRate = vk::VertexInputRate::eVertex;
	vBinding.binding = 0;

	vk::VertexInputAttributeDescription attributes[2];

	attributes[0].binding = 0;
	attributes[0].format = vk::Format::eR32G32B32Sfloat;
	attributes[0].location = 0;
	attributes[0].offset = 0;

	attributes[1].binding = 0;
	attributes[1].format = vk::Format::eR32G32B32Sfloat;
	attributes[1].location = 1;
	attributes[1].offset = 12;

	vk::PipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = vk::StructureType::ePipelineVertexInputStateCreateInfo;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &vBinding;
	vertexInputInfo.vertexAttributeDescriptionCount = 2;
	vertexInputInfo.pVertexAttributeDescriptions = attributes;


	vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
	inputAssemblyInfo.sType = vk::StructureType::ePipelineInputAssemblyStateCreateInfo;
	inputAssemblyInfo.topology = vk::PrimitiveTopology::eTriangleList;
	inputAssemblyInfo.primitiveRestartEnable = false;

	//View of the screen
	vk::Viewport viewport;
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = (float)m_extent.width;
	viewport.height = (float)m_extent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	vk::Rect2D scissors;
	scissors.offset.x = 0;
	scissors.offset.y = 0;
	scissors.extent = m_extent;

	vk::PipelineViewportStateCreateInfo viewportStateInfo{};
	viewportStateInfo.sType = vk::StructureType::ePipelineViewportStateCreateInfo;
	viewportStateInfo.pScissors = &scissors;
	viewportStateInfo.pViewports = &viewport;
	viewportStateInfo.scissorCount = 1;
	viewportStateInfo.viewportCount = 1;

	//Rasterizer
	vk::PipelineRasterizationStateCreateInfo rasterizerInfo{};
	rasterizerInfo.sType = vk::StructureType::ePipelineRasterizationStateCreateInfo;
	rasterizerInfo.depthClampEnable = false;
	rasterizerInfo.rasterizerDiscardEnable = false;
	rasterizerInfo.polygonMode = vk::PolygonMode::eFill;
	rasterizerInfo.lineWidth = 1.0f;
	rasterizerInfo.cullMode = vk::CullModeFlagBits::eNone;
	rasterizerInfo.frontFace = vk::FrontFace::eClockwise;
	rasterizerInfo.depthBiasEnable = false;
	rasterizerInfo.depthBiasConstantFactor = 0.0f;
	rasterizerInfo.depthBiasClamp = 0.0f;
	rasterizerInfo.depthBiasSlopeFactor = 0.0f;

	//Multisampling
	vk::PipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = vk::StructureType::ePipelineMultisampleStateCreateInfo;
	multisampling.sampleShadingEnable = false;
	multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
	multisampling.minSampleShading = 1.0f; // Optional
	multisampling.pSampleMask = nullptr; // Optional
	multisampling.alphaToCoverageEnable = false; // Optional
	multisampling.alphaToOneEnable = false; // Optional

	//Color Attachments
	vk::PipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
	colorBlendAttachment.blendEnable = false;

	vk::PipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = vk::StructureType::ePipelineColorBlendStateCreateInfo;
	colorBlending.logicOpEnable = false;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;

	//Layout
	vk::PipelineLayoutCreateInfo pipelineLayout{};
	pipelineLayout.sType = vk::StructureType::ePipelineLayoutCreateInfo;
	pipelineLayout.pSetLayouts = &m_descriptorLayout;
	pipelineLayout.setLayoutCount = 1;
	m_pipelineLayout = VulkanEngine::LogicalDevice.createPipelineLayout(pipelineLayout);


	std::vector<vk::DynamicState> dynamicStates = {
	vk::DynamicState::eViewport,
	vk::DynamicState::eScissor
	};

	vk::PipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = vk::StructureType::ePipelineDynamicStateCreateInfo;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();

	vk::GraphicsPipelineCreateInfo graphicsPipeline{};
	graphicsPipeline.sType = vk::StructureType::eGraphicsPipelineCreateInfo;
	graphicsPipeline.stageCount = 2;
	graphicsPipeline.pStages = createInfos;
	graphicsPipeline.pVertexInputState = &vertexInputInfo;
	graphicsPipeline.pInputAssemblyState = &inputAssemblyInfo;
	graphicsPipeline.pViewportState = &viewportStateInfo;
	graphicsPipeline.pRasterizationState = &rasterizerInfo;
	graphicsPipeline.pMultisampleState = &multisampling;
	graphicsPipeline.pColorBlendState = &colorBlending;
	graphicsPipeline.pDynamicState = &dynamicState;
	graphicsPipeline.layout = m_pipelineLayout;
	graphicsPipeline.renderPass = m_renderPass;
	graphicsPipeline.subpass = 0;

	m_pipeline = VulkanEngine::LogicalDevice.createGraphicsPipeline(nullptr, graphicsPipeline).value;

	VulkanEngine::LogicalDevice.destroyShaderModule(vertexModule);
	VulkanEngine::LogicalDevice.destroyShaderModule(fragmentModule);
}

void VulkanPipeline::CreateFrameBuffers()
{
	m_frameBuffers.reserve(m_imageViews.size());

	for (size_t i = 0; i < m_imageViews.size(); i++)
	{
		//std::vector<ImageView>
		vk::ImageView* attachment = &m_imageViews[i];

		vk::FramebufferCreateInfo createInfo{};
		createInfo.sType = vk::StructureType::eFramebufferCreateInfo;
		createInfo.renderPass = m_renderPass;
		createInfo.height = m_extent.height;
		createInfo.width = m_extent.width;
		createInfo.attachmentCount = 1;
		createInfo.pAttachments = attachment;
		createInfo.layers = 1;
		m_frameBuffers.push_back(VulkanEngine::LogicalDevice.createFramebuffer(createInfo));
	}
}

void VulkanPipeline::CreateVertexBuffer()
{
	uint64_t bufferSize = sizeof(Vertex) * m_vertices.size();

	vk::Buffer stagingBuffer;
	vk::DeviceMemory stagingMemory;

	VertexBufferInfo staging = {
		.buffer = stagingBuffer,
		.memory = stagingMemory,
		.usage = vk::BufferUsageFlagBits::eTransferSrc,
		.properties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
		.size = bufferSize
	};

	vhf::CreateBuffer(staging);
	void* map = VulkanEngine::LogicalDevice.mapMemory(stagingMemory, 0, bufferSize, vk::MemoryMapFlagBits::ePlacedEXT);
	memcpy(map, m_vertices.data(), bufferSize);
	VulkanEngine::LogicalDevice.unmapMemory(stagingMemory);
	map = nullptr;

	VertexBufferInfo vertexBuffer = {
		.buffer = m_vertexBuffer,
		.memory = m_vertexMemory,
		.usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
		.properties = vk::MemoryPropertyFlagBits::eDeviceLocal,
		.size = bufferSize
	};

	vhf::CreateBuffer(vertexBuffer);

	vhf::CopyBufferToBuffer(stagingBuffer, m_vertexBuffer, bufferSize, VulkanEngine::GraphicsQueue);

	VulkanEngine::LogicalDevice.freeMemory(stagingMemory);
	VulkanEngine::LogicalDevice.destroyBuffer(stagingBuffer);
}

void VulkanPipeline::CreateIndexBuffer()
{
	uint64_t bufferSize = sizeof(unsigned short) *  m_indices.size();
	
	vk::Buffer stagingBuffer;
	vk::DeviceMemory stagingMemory;
	
	VertexBufferInfo staging = {
		.buffer = stagingBuffer,
		.memory = stagingMemory,
		.usage = vk::BufferUsageFlagBits::eTransferSrc,
		.properties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
		.size = bufferSize
	};
	
	vhf::CreateBuffer(staging);
	void* map = VulkanEngine::LogicalDevice.mapMemory(stagingMemory, 0, bufferSize, vk::MemoryMapFlagBits::ePlacedEXT);
	memcpy(map, m_indices.data(), bufferSize);
	VulkanEngine::LogicalDevice.unmapMemory(stagingMemory);
	map = nullptr;
	
	VertexBufferInfo vertexBuffer = {
		.buffer = m_indexBuffer,
		.memory = m_indexMemory,
		.usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
		.properties = vk::MemoryPropertyFlagBits::eDeviceLocal,
		.size = bufferSize
	};
	
	vhf::CreateBuffer(vertexBuffer);
	
	vhf::CopyBufferToBuffer(stagingBuffer, m_indexBuffer, bufferSize, VulkanEngine::GraphicsQueue);
	
	VulkanEngine::LogicalDevice.freeMemory(stagingMemory);
	VulkanEngine::LogicalDevice.destroyBuffer(stagingBuffer);
}

void VulkanPipeline::AddMesh(aiMesh* _mesh)
{
	int baseIndex = m_triangleCount * 3;
	int baseVertexIndex = m_vertexCount;

	m_vertexCount += _mesh->mNumVertices;
	m_vertices.reserve(m_vertexCount);

	for (int i = 0; i < _mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.r = rand() / RAND_MAX;
		vertex.g = rand() / RAND_MAX;
		vertex.b = rand() / RAND_MAX;

		memcpy(&vertex, &_mesh->mVertices[i], sizeof(aiVector3D));

		m_vertices.push_back(vertex);
	}

	m_triangleCount += _mesh->mNumFaces;
	m_indices.reserve(m_indices.size() + _mesh->mNumFaces);
	for (int i = 0; i < _mesh->mNumFaces; i++)
	{
		aiFace& face = _mesh->mFaces[i];
		m_indices.push_back(face.mIndices[0]);
		m_indices.push_back(face.mIndices[1]);
		m_indices.push_back(face.mIndices[2]);
	}
}

vk::ShaderModule VulkanPipeline::WrapShader(std::vector<char> _shaderBytes)
{
	vk::ShaderModuleCreateInfo info{};
	info.sType = vk::StructureType::eShaderModuleCreateInfo;
	info.codeSize = _shaderBytes.size();
	info.pCode = reinterpret_cast<const uint32_t*>(_shaderBytes.data());

	return 	VulkanEngine::LogicalDevice.createShaderModule(info);
}

vk::SurfaceFormatKHR VulkanPipeline::GetFormat(std::vector<vk::SurfaceFormatKHR>& _format)
{
	return _format[0];
}

vk::Extent2D VulkanPipeline::GetExtent(vk::SurfaceCapabilitiesKHR _capabilities)
{
	if (_capabilities.currentExtent != std::numeric_limits<uint32_t>::max())
		return _capabilities.currentExtent;

	int width = 800;
	int height = 800;
	//glfwGetFramebufferSize(m_window, &width, &height);

	uint32_t uwidth = (uint32_t)width;
	uint32_t uheight = (uint32_t)height;

	uwidth = std::clamp(uwidth, _capabilities.minImageExtent.width, _capabilities.maxImageExtent.width);
	uheight = std::clamp(uheight, _capabilities.minImageExtent.height, _capabilities.maxImageExtent.height);

	return vk::Extent2D(uwidth, uheight);
}