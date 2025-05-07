#include "Renderer/VulkanData.h"
#include "Renderer/VulkanHelperFunctions.h"
#include "Renderer/Renderer.h"

void MaterialInstance::AllocateSets(vk::Device _device, vk::DescriptorSetLayout* _pSetLayouts, vk::DescriptorPool _descriptorPool, int _count)
{
	m_descriptorSets = new vk::DescriptorSet[_count];
	m_descriptorSetCount = _count;

	vk::DescriptorSetAllocateInfo allocInfo;
	allocInfo.sType = vk::StructureType::eDescriptorSetAllocateInfo;
	allocInfo.descriptorPool = _descriptorPool;
	allocInfo.descriptorSetCount = _count;
	allocInfo.pSetLayouts = _pSetLayouts;
	
	_device.allocateDescriptorSets(&allocInfo, m_descriptorSets);
}

void MaterialInstance::UpdateSets(vk::Device _device, std::vector<vk::ImageView> _views, std::vector<vk::Sampler> _samplers)
{
	std::vector<vk::WriteDescriptorSet> writeSets;
	writeSets.reserve(m_descriptorSetCount);

	for (int i = 0; i < m_descriptorSetCount; i++)
	{
		vk::DescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		imageInfo.imageView = _views[i];
		imageInfo.sampler = _samplers[i];

		vk::WriteDescriptorSet combinedSamplerSet;
		combinedSamplerSet.sType = vk::StructureType::eWriteDescriptorSet;
		combinedSamplerSet.dstSet = m_descriptorSets[i];
		combinedSamplerSet.dstBinding = i;
		combinedSamplerSet.dstArrayElement = 0;
		combinedSamplerSet.descriptorType = vk::DescriptorType::eCombinedImageSampler;
		combinedSamplerSet.descriptorCount = 1;
		combinedSamplerSet.pImageInfo = &imageInfo;

		writeSets.push_back(combinedSamplerSet);
	}

	_device.updateDescriptorSets(writeSets.size(), writeSets.data(), 0, nullptr);
}

void MaterialInstance::BindSets(vk::CommandBuffer _buffer, vk::PipelineLayout _layout)
{
	_buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, _layout, 1, m_descriptorSetCount, m_descriptorSets, 0, nullptr);
}

void MaterialInstance::CleanUp(vk::Device _device, vk::DescriptorPool _pool)
{
	_device.freeDescriptorSets(_pool, m_descriptorSetCount, m_descriptorSets);
}

void MaterialInstance::RecordCommandBuffer(vk::CommandBuffer& _buffer, int _renderPass, vk::PipelineBindPoint _bindPoint)
{
	m_material.RecordCommandBuffer(_buffer, _renderPass, _bindPoint);
	BindSets(_buffer, m_material.GetLayouts()[0]);
}

Material::Material(VulkanRenderer* _renderer, vk::Device _device, int _textureCount)
{
	m_pipelines.resize(2);
	CreatePipelines(_renderer, _device);
	m_layouts.resize(1);
	m_layouts[0] = _renderer->GetPipelineLayout();
	m_textureCount = _textureCount;
}

void Material::CreatePipelines(VulkanRenderer* _renderer, vk::Device _device)
{
	vk::GraphicsPipelineCreateInfo pipelineInfo;
	pipelineInfo.sType = vk::StructureType::eGraphicsPipelineCreateInfo;

#pragma region Stages
	vk::PipelineShaderStageCreateInfo* shaderStages = new vk::PipelineShaderStageCreateInfo[2];

	auto vertexShaderBytes = readFile("Shaders/vert.spv");
	vk::ShaderModule module = vhf::WrapShader(vertexShaderBytes);
	shaderStages[0].sType = vk::StructureType::ePipelineShaderStageCreateInfo;
	shaderStages[0].stage = vk::ShaderStageFlagBits::eVertex;
	shaderStages[0].module = module;
	shaderStages[0].pName = "main";
	

	auto fragmentShaderBytes = readFile("Shaders/frag.spv");
	vk::ShaderModule fragModule = vhf::WrapShader(fragmentShaderBytes);
	shaderStages[1].sType = vk::StructureType::ePipelineShaderStageCreateInfo;
	shaderStages[1].stage = vk::ShaderStageFlagBits::eFragment;
	shaderStages[1].module = fragModule;
	shaderStages[1].pName = "main";

	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
#pragma endregion

#pragma region Inputs

#pragma region Vertex
	vk::VertexInputBindingDescription* vertexBinding = new vk::VertexInputBindingDescription;
	vertexBinding[0].binding = 0;
	vertexBinding[0].stride = 32;
	vertexBinding[0].inputRate = vk::VertexInputRate::eVertex;

	vk::VertexInputAttributeDescription* vertexAttributes = new vk::VertexInputAttributeDescription[3];

	vertexAttributes[0].binding = 0;
	vertexAttributes[0].format = vk::Format::eR32G32B32Sfloat;
	vertexAttributes[0].location = 0;
	vertexAttributes[0].offset = 0;

	vertexAttributes[1].binding = 0;
	vertexAttributes[1].format = vk::Format::eR32G32B32Sfloat;
	vertexAttributes[1].location = 1;
	vertexAttributes[1].offset = 12;
	
	vertexAttributes[2].binding = 0;
	vertexAttributes[2].format = vk::Format::eR32G32Sfloat;
	vertexAttributes[2].location = 2;
	vertexAttributes[2].offset = 24;

	vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = 3;
	vertexInputInfo.pVertexAttributeDescriptions = vertexAttributes;
	vertexInputInfo.pVertexBindingDescriptions = vertexBinding;

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

#pragma region Depth
	vk::PipelineDepthStencilStateCreateInfo* _depthStates = new vk::PipelineDepthStencilStateCreateInfo[2];

	_depthStates[0].sType = vk::StructureType::ePipelineDepthStencilStateCreateInfo;
	_depthStates[0].depthCompareOp = vk::CompareOp::eLessOrEqual;
	_depthStates[0].depthTestEnable = true;
	_depthStates[0].depthWriteEnable = true;
	_depthStates[0].stencilTestEnable = false;

	_depthStates[1].sType = vk::StructureType::ePipelineDepthStencilStateCreateInfo;
	_depthStates[1].depthCompareOp = vk::CompareOp::eLessOrEqual;
	_depthStates[1].depthTestEnable = true;
	_depthStates[1].depthWriteEnable = false;
	_depthStates[1].stencilTestEnable = false;

	pipelineInfo.pDepthStencilState = &_depthStates[0];
#pragma endregion

#pragma region PipelineCreation
	pipelineInfo.layout = _renderer->GetPipelineLayout();
	pipelineInfo.subpass = 0;

	pipelineInfo.renderPass = _renderer->GetRenderPass();

	m_pipelines[0] = _device.createGraphicsPipeline(nullptr, pipelineInfo).value;

	pipelineInfo.subpass = 1;
	pipelineInfo.pDepthStencilState = &_depthStates[1];

	m_pipelines[1] = _device.createGraphicsPipeline(nullptr, pipelineInfo).value;
#pragma endregion

	_device.destroyShaderModule(module);
	_device.destroyShaderModule(fragModule);
}

MaterialInstance* Material::CreateInstance(vk::Device _device, vk::DescriptorSetLayout* _pSetLayouts, vk::DescriptorPool _pool, int _layoutCount)
{
	MaterialInstance* instance = new MaterialInstance(*this);

	instance->AllocateSets(_device, _pSetLayouts, _pool, _layoutCount);

	return instance;
}

void Material::RecordCommandBuffer(vk::CommandBuffer _buffer, int _renderPass, vk::PipelineBindPoint _bindPoint)
{
	_buffer.bindPipeline(_bindPoint, m_pipelines[_renderPass]);
}
