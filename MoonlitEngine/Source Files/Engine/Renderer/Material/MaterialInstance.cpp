#include "Engine/Renderer/Material/MaterialInstance.h"
#include "Engine/Renderer/RenderTarget.h"
#include "Engine/Renderer/Material/Material.h"
#include "Engine/Renderer/VulkanHelperFunctions.h"

#include "Engine/Renderer/HelperClasses/VertexInputHelper.h"

MaterialInstance::MaterialInstance(RenderTarget& _target, Material* _material)
	: m_target(_target), m_baseMaterial(_material)
{
	m_shaderData = m_baseMaterial->GetShaderData();
	m_deviceData = m_target.GetDeviceData();
	//TODO: Handle multiple textures
	m_textureCount = 1;
	CreatePipelineLayouts();
	CreatePipelines();
	CreateDescriptorPool();
}

MaterialInstance::~MaterialInstance()
{
	for (auto& pipeline : m_pipelines)
	{
		m_deviceData.Device.destroyPipeline(pipeline.second);
	}
	for (auto& layout : m_pipelineLayouts)
	{
		m_deviceData.Device.destroyPipelineLayout(layout);
	}
	for (auto& setLayout : m_setLayouts)
	{
		m_deviceData.Device.destroyDescriptorSetLayout(setLayout);
	}
	m_deviceData.Device.destroyDescriptorPool(m_descriptorPool);

	m_baseMaterial->RemoveInstance(this);
}

void MaterialInstance::CreatePipelineLayouts()
{
	vk::DescriptorSetLayout uboLayout = m_target.GetUBODescriptorSetLayout();

	m_pipelineLayouts.resize(1);

	m_setLayouts.resize(2);
	m_setLayouts[0] = uboLayout;

	vk::DescriptorSetLayoutBinding textureLayoutBinding;
	textureLayoutBinding.binding = 0;
	textureLayoutBinding.descriptorType = vk::DescriptorType::eCombinedImageSampler;
	textureLayoutBinding.descriptorCount = TextureArrayCount;
	textureLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eFragment;
	textureLayoutBinding.pImmutableSamplers = nullptr;

	vk::DescriptorSetLayoutCreateInfo setLayoutCreateInfo;
	setLayoutCreateInfo.sType = vk::StructureType::eDescriptorSetLayoutCreateInfo;
	setLayoutCreateInfo.bindingCount = 1;
	setLayoutCreateInfo.pBindings = &textureLayoutBinding;

	const vk::DescriptorBindingFlags flags = vk::DescriptorBindingFlagBits::ePartiallyBound;

	vk::DescriptorSetLayoutBindingFlagsCreateInfo flagsCreateInfo = vk::DescriptorSetLayoutBindingFlagsCreateInfo();
	flagsCreateInfo.sType = vk::StructureType::eDescriptorSetLayoutBindingFlagsCreateInfo;
	flagsCreateInfo.bindingCount = 1;
	flagsCreateInfo.pBindingFlags = &flags;

	setLayoutCreateInfo.pNext = &flagsCreateInfo;

	m_setLayouts[1] = m_deviceData.Device.createDescriptorSetLayout(setLayoutCreateInfo);

	vk::PipelineLayoutCreateInfo pipelineLayout{};
	pipelineLayout.sType = vk::StructureType::ePipelineLayoutCreateInfo;
	pipelineLayout.pSetLayouts = m_setLayouts.data();
	pipelineLayout.setLayoutCount = 2;
	m_pipelineLayouts[0] = m_deviceData.Device.createPipelineLayout(pipelineLayout);
}

vk::PipelineDepthStencilStateCreateInfo GetDepthStencilState(std::string subpass)
{
	// Default depth stencil state
	vk::PipelineDepthStencilStateCreateInfo depthStencil;
	depthStencil.sType = vk::StructureType::ePipelineDepthStencilStateCreateInfo;
	depthStencil.depthTestEnable = true;
	depthStencil.depthWriteEnable = true;
	depthStencil.depthCompareOp = vk::CompareOp::eLess;
	depthStencil.depthBoundsTestEnable = false;
	depthStencil.minDepthBounds = 0.0f;
	depthStencil.maxDepthBounds = 1.0f;
	depthStencil.stencilTestEnable = false;

	// Modify based on subpass type
	// We can create an override system later if needed
	if (subpass == "color")
	{
		depthStencil.depthWriteEnable = false;
		depthStencil.depthCompareOp = vk::CompareOp::eLessOrEqual;
	}

	return depthStencil;
}

void MaterialInstance::CreatePipelines()
{
	vk::RenderPass _renderPass = m_target.GetRenderPass();
	std::vector<std::string> subpassNames = m_baseMaterial->GetIncludedSubpasses();



	vk::GraphicsPipelineCreateInfo pipelineInfo;
	pipelineInfo.sType = vk::StructureType::eGraphicsPipelineCreateInfo;

#pragma region Inputs

#pragma region Vertex

	VertexInputBuilder  vertexInputBuilder;
	vertexInputBuilder.AddBinding(vk::VertexInputRate::eVertex)
		.AddAttribute(vk::Format::eR32G32B32Sfloat, 12)   // Position
		.AddAttribute(vk::Format::eR32G32Sfloat, 8)        // UV
		.AddAttribute(vk::Format::eR32G32B32Sfloat, 12)    // Normal
		.AddAttribute(vk::Format::eR32G32B32Sfloat, 12)    // Tangent
		.AddAttribute(vk::Format::eR32G32B32Sfloat, 12);   // Bitangent

	vertexInputBuilder.AddBinding(vk::VertexInputRate::eInstance)
		.AddAttribute(vk::Format::eR32G32B32A32Sfloat, 16) // Model Matrix Row 0
		.AddAttribute(vk::Format::eR32G32B32A32Sfloat, 16) // Model Matrix Row 1
		.AddAttribute(vk::Format::eR32G32B32A32Sfloat, 16) // Model Matrix Row 2
		.AddAttribute(vk::Format::eR32G32B32A32Sfloat, 16) // Model Matrix Row 3
		.AddAttribute(vk::Format::eR32G32B32A32Sfloat, 16) // Inverse Model Matrix Row 0
		.AddAttribute(vk::Format::eR32G32B32A32Sfloat, 16) // Inverse Model Matrix Row 1
		.AddAttribute(vk::Format::eR32G32B32A32Sfloat, 16) // Inverse Model Matrix Row 2
		.AddAttribute(vk::Format::eR32G32B32A32Sfloat, 16); // Inverse Model Matrix Row 3

	for (int i = 0; i < m_textureCount; i++)
	{
		vertexInputBuilder.AddAttribute(vk::Format::eR32Sint, 4); // Texture Indices
	}

	VertexInputDescription vertexInputDescription = vertexInputBuilder.Build();

	vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
	vertexInputInfo.vertexBindingDescriptionCount = vertexInputDescription.Bindings.size();
	vertexInputInfo.vertexAttributeDescriptionCount = vertexInputDescription.Attributes.size();
	vertexInputInfo.pVertexAttributeDescriptions = vertexInputDescription.Attributes.data();
	vertexInputInfo.pVertexBindingDescriptions = vertexInputDescription.Bindings.data();

	pipelineInfo.pVertexInputState = &vertexInputInfo;
#pragma endregion

	vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
	inputAssembly.sType = vk::StructureType::ePipelineInputAssemblyStateCreateInfo;
	inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
	inputAssembly.primitiveRestartEnable = false;

	pipelineInfo.pInputAssemblyState = &inputAssembly;
#pragma endregion

#pragma region DynamicStates
	vk::DynamicState* dynamicStates = new vk::DynamicState[2]{ vk::DynamicState::eViewport, vk::DynamicState::eScissor };

	vk::PipelineViewportStateCreateInfo viewportState;
	viewportState.sType = vk::StructureType::ePipelineViewportStateCreateInfo;
	viewportState.pViewports = nullptr;
	viewportState.pScissors = nullptr;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;

	pipelineInfo.pViewportState = &viewportState;

	vk::PipelineDynamicStateCreateInfo dynamicStateCreate;
	dynamicStateCreate.sType = vk::StructureType::ePipelineDynamicStateCreateInfo;
	dynamicStateCreate.dynamicStateCount = 2;
	dynamicStateCreate.pDynamicStates = dynamicStates;

	pipelineInfo.pDynamicState = &dynamicStateCreate;

#pragma endregion

#pragma region Rasterizer
	vk::PipelineRasterizationStateCreateInfo rasterizer;
	rasterizer.sType = vk::StructureType::ePipelineRasterizationStateCreateInfo;
	rasterizer.depthClampEnable = false;
	rasterizer.rasterizerDiscardEnable = false;
	rasterizer.depthBiasEnable = false;

	rasterizer.polygonMode = vk::PolygonMode::eFill;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = vk::CullModeFlagBits::eBack;
	rasterizer.frontFace = vk::FrontFace::eClockwise;

	rasterizer.depthBiasConstantFactor = 0.0f;
	rasterizer.depthBiasClamp = 0.0f;
	rasterizer.depthBiasSlopeFactor = 0.0f;

	pipelineInfo.pRasterizationState = &rasterizer;
#pragma endregion

#pragma region Multisampling
	vk::PipelineMultisampleStateCreateInfo multisampling;
	multisampling.sType = vk::StructureType::ePipelineMultisampleStateCreateInfo;
	multisampling.sampleShadingEnable = false;
	multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
	multisampling.minSampleShading = 1.0f;
	multisampling.pSampleMask = nullptr;
	multisampling.alphaToCoverageEnable = false;
	multisampling.alphaToOneEnable = false;

	pipelineInfo.pMultisampleState = &multisampling;
#pragma endregion

#pragma region Color
	vk::PipelineColorBlendAttachmentState colorBlendAttachment;
	colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
	colorBlendAttachment.blendEnable = false;

	vk::PipelineColorBlendStateCreateInfo colorBlendState;
	colorBlendState.sType = vk::StructureType::ePipelineColorBlendStateCreateInfo;
	colorBlendState.logicOpEnable = false;
	colorBlendState.attachmentCount = 1;
	colorBlendState.pAttachments = &colorBlendAttachment;

	pipelineInfo.pColorBlendState = &colorBlendState;
#pragma endregion

	pipelineInfo.renderPass = _renderPass;
	pipelineInfo.layout = m_pipelineLayouts[0];

#pragma region SubpassDependancy
	// We know how many subpasses there are from the material
	std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;
	std::vector<vk::ShaderModule> shaderModules;

	for (auto& subpassName : subpassNames)
	{
		shaderStages.clear();
		shaderModules.clear();

		for (auto& entryPoint : m_shaderData.EntryPoints)
		{
			auto it = std::find(entryPoint.SubpassNames.begin(), entryPoint.SubpassNames.end(), subpassName);
			if (it == entryPoint.SubpassNames.end())
				continue;

			shaderModules.emplace_back(vhf::WrapShader(m_deviceData.Device, entryPoint.Function.Code.CodePtr, entryPoint.Function.Code.Size));
			vk::PipelineShaderStageCreateInfo shaderStageInfo;
			shaderStageInfo.sType = vk::StructureType::ePipelineShaderStageCreateInfo;
			shaderStageInfo.stage = entryPoint.Stage;
			shaderStageInfo.module = shaderModules.back();
			shaderStageInfo.pName = "main";
			shaderStages.push_back(shaderStageInfo);
		}

		vk::PipelineDepthStencilStateCreateInfo depthStencilState = GetDepthStencilState(subpassName);
		pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
		pipelineInfo.pStages = shaderStages.data();
		pipelineInfo.pDepthStencilState = &depthStencilState;
		pipelineInfo.subpass = m_target.GetSubpassIndexByName(subpassName);

		m_pipelines[subpassName] = m_deviceData.Device.createGraphicsPipeline(nullptr, pipelineInfo).value;

		for (auto& module : shaderModules)
		{
			m_deviceData.Device.destroyShaderModule(module);
		}
	}
#pragma endregion
}

void MaterialInstance::CreateDescriptorPool()
{
	vk::DescriptorPoolSize poolSize;
	poolSize.type = vk::DescriptorType::eCombinedImageSampler;
	poolSize.descriptorCount = TextureArrayCount;

	vk::DescriptorPoolCreateInfo poolInfo;
	poolInfo.sType = vk::StructureType::eDescriptorPoolCreateInfo;
	poolInfo.maxSets = 1;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;
	m_descriptorPool = m_deviceData.Device.createDescriptorPool(poolInfo);
}

void MaterialInstance::RecordCommandBuffer(vk::CommandBuffer _buffer, std::string _renderPass, vk::PipelineBindPoint _bindPoint)
{
	vk::Pipeline _pipeline = m_pipelines[_renderPass];
	_buffer.bindPipeline(_bindPoint, _pipeline);
}
