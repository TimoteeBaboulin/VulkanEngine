#include "Renderer/Material.h"

#include "Renderer/VulkanHelperFunctions.h"
#include "Renderer/Renderer.h"
#include "slang/slang-com-ptr.h"

#include "ResourceManagement/FileHelper.h"

Material::Material(Renderer* _renderer, vk::Device _device, int _textureCount)
{
	m_textureCount = _textureCount;
	CreatePipelineLayouts(*_renderer, _device);
	m_pipelines.resize(2);
	CreatePipelines(_renderer, _device);
}

struct ShaderCode
{
	void* code;
	size_t size;
};

ShaderCode* GetModules(const char* filepath)
{
	Slang::ComPtr<slang::IGlobalSession> globalSession;
	SlangGlobalSessionDesc globalSessionDesc;
	slang::createGlobalSession(&globalSessionDesc, globalSession.writeRef());

	slang::SessionDesc sessionDesc;
	sessionDesc.structureSize = sizeof(slang::SessionDesc);
	//sessionDesc.defaultMatrixLayoutMode = SLANG_MATRIX_LAYOUT_ROW_MAJOR;

	//Targets
	slang::TargetDesc targets[1];
	targets[0].format = SLANG_SPIRV;
	targets[0].profile = globalSession.get()->findProfile("glsl_450");
	sessionDesc.targets = targets;
	sessionDesc.targetCount = 1;

	sessionDesc.searchPaths = nullptr;
	sessionDesc.searchPathCount = 0;

	sessionDesc.preprocessorMacroCount = 0;
	sessionDesc.preprocessorMacros = nullptr;

	Slang::ComPtr<slang::ISession> sessionPtr;
	globalSession.get()->createSession(sessionDesc, sessionPtr.writeRef());

	Slang::ComPtr<slang::IBlob> diagnostics;

	slang::IModule* module = sessionPtr.get()->loadModule(filepath, diagnostics.writeRef());

	if (diagnostics)
	{
		fprintf(stderr, "%s\n", (const char*)diagnostics->getBufferPointer());
	}

	Slang::ComPtr<slang::IEntryPoint> vertEntryPoint;
	Slang::ComPtr<slang::IEntryPoint> fragEntryPoint;

	module->findEntryPointByName("vertexMain", vertEntryPoint.writeRef());

	module->findEntryPointByName("fragmentMain", fragEntryPoint.writeRef());

	slang::IComponentType* componentTypes[] = { module, vertEntryPoint, fragEntryPoint };
	Slang::ComPtr<slang::IComponentType> program;
	sessionPtr.get()->createCompositeComponentType(componentTypes, 3, program.writeRef());

	
	Slang::ComPtr<slang::IComponentType> linkedProgram;
	program.get()->link(linkedProgram.writeRef(), diagnostics.writeRef());

	Slang::ComPtr<slang::IBlob> blobs[2];

	linkedProgram.get()->getEntryPointCode(0, 0, blobs[0].writeRef(), diagnostics.writeRef());
	if (diagnostics)
	{
		fprintf(stderr, "%s\n", (const char*)diagnostics->getBufferPointer());
	}
	linkedProgram.get()->getEntryPointCode(1, 0, blobs[1].writeRef(), diagnostics.writeRef());
	if (diagnostics)
	{
		fprintf(stderr, "%s\n", (const char*)diagnostics->getBufferPointer());
	}

	ShaderCode* shaderCodes = new ShaderCode[2];

	shaderCodes[0].size = blobs[0].get()->getBufferSize();
	shaderCodes[0].code = malloc(shaderCodes[0].size);
	memcpy(shaderCodes[0].code, blobs[0].get()->getBufferPointer(), shaderCodes[0].size);
	
	shaderCodes[1].size = blobs[1].get()->getBufferSize();
	shaderCodes[1].code = malloc(shaderCodes[1].size);
	memcpy(shaderCodes[1].code, blobs[1].get()->getBufferPointer(), shaderCodes[1].size);

	return shaderCodes;
}

void Material::CreatePipelines(Renderer* _renderer, vk::Device _device)
{
	vk::GraphicsPipelineCreateInfo pipelineInfo;
	pipelineInfo.sType = vk::StructureType::eGraphicsPipelineCreateInfo;

	ShaderCode* shaderCodes = GetModules("Shaders/BaseMaterial.slang");


#pragma region Stages
	vk::PipelineShaderStageCreateInfo* shaderStages = new vk::PipelineShaderStageCreateInfo[2];

	//auto vertexShaderBytes = readFile("Shaders/vert.spv");
	vk::ShaderModule module = vhf::WrapShader(_device, shaderCodes[0].code, shaderCodes[0].size);
	shaderStages[0].sType = vk::StructureType::ePipelineShaderStageCreateInfo;
	shaderStages[0].stage = vk::ShaderStageFlagBits::eVertex;
	shaderStages[0].module = module;
	shaderStages[0].pName = "main";
	
	pipelineInfo.stageCount = 1;

	pipelineInfo.pStages = shaderStages;
#pragma endregion

#pragma region Inputs

#pragma region Vertex
	vk::VertexInputBindingDescription* vertexBinding = new vk::VertexInputBindingDescription[2];
	vertexBinding[0].binding = 0;
	vertexBinding[0].stride = 32;
	vertexBinding[0].inputRate = vk::VertexInputRate::eVertex;

	vertexBinding[1].binding = 1;
	vertexBinding[1].stride = 64 + 4 * m_textureCount;
	vertexBinding[1].inputRate = vk::VertexInputRate::eInstance;

	vk::VertexInputAttributeDescription* vertexAttributes = new vk::VertexInputAttributeDescription[7 + m_textureCount];

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

	vertexAttributes[3].binding = 1;
	vertexAttributes[3].format = vk::Format::eR32G32B32A32Sfloat;
	vertexAttributes[3].location = 3;
	vertexAttributes[3].offset = 0;

	vertexAttributes[4].binding = 1;
	vertexAttributes[4].format = vk::Format::eR32G32B32A32Sfloat;
	vertexAttributes[4].location = 4;
	vertexAttributes[4].offset = 16;

	vertexAttributes[5].binding = 1;
	vertexAttributes[5].format = vk::Format::eR32G32B32A32Sfloat;
	vertexAttributes[5].location = 5;
	vertexAttributes[5].offset = 32;

	vertexAttributes[6].binding = 1;
	vertexAttributes[6].format = vk::Format::eR32G32B32A32Sfloat;
	vertexAttributes[6].location = 6;
	vertexAttributes[6].offset = 48;

	int offset = 64;

	for (size_t index = 7; index < 7 + m_textureCount; index++)
	{
		vertexAttributes[index].binding = 1;
		vertexAttributes[index].format = vk::Format::eR32Sint;
		vertexAttributes[index].location = index;
		vertexAttributes[index].offset = offset;

		offset += 4;
	}

	vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
	vertexInputInfo.vertexBindingDescriptionCount = 2;
	vertexInputInfo.vertexAttributeDescriptionCount = 7 + m_textureCount;
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
	pipelineInfo.layout = m_pipelineLayouts[0];
	pipelineInfo.subpass = 0;

	pipelineInfo.renderPass = _renderer->GetRenderPass();

	m_pipelines[0] = _device.createGraphicsPipeline(nullptr, pipelineInfo).value;

	auto fragmentShaderBytes = FileHelper::ReadFile("Shaders/frag.spv");
	vk::ShaderModule fragModule = vhf::WrapShader(_device, shaderCodes[1].code, shaderCodes[1].size);
	shaderStages[1].sType = vk::StructureType::ePipelineShaderStageCreateInfo;
	shaderStages[1].stage = vk::ShaderStageFlagBits::eFragment;
	shaderStages[1].module = fragModule;
	shaderStages[1].pName = "main";

	pipelineInfo.stageCount = 2;

	pipelineInfo.subpass = 1;
	pipelineInfo.pDepthStencilState = &_depthStates[1];

	m_pipelines[1] = _device.createGraphicsPipeline(nullptr, pipelineInfo).value;
#pragma endregion

	_device.destroyShaderModule(module);
	_device.destroyShaderModule(fragModule);
}

void Material::CreatePipelineLayouts(Renderer& _renderer, vk::Device& _device)
{
	m_pipelineLayouts.resize(1);

	m_setLayouts.resize(2);
	m_setLayouts[0] = _renderer.GetUboDescriptorSetLayout();

	//vk::DescriptorSetLayoutBinding* textureBindings = new vk::DescriptorSetLayoutBinding[m_textureCount];

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

	m_setLayouts[1] = _device.createDescriptorSetLayout(setLayoutCreateInfo);

	vk::PipelineLayoutCreateInfo pipelineLayout{};
	pipelineLayout.sType = vk::StructureType::ePipelineLayoutCreateInfo;
	pipelineLayout.pSetLayouts = m_setLayouts.data();
	pipelineLayout.setLayoutCount = 2;
	m_pipelineLayouts[0] = _device.createPipelineLayout(pipelineLayout);
}

void Material::RecordCommandBuffer(vk::CommandBuffer _buffer, int _renderPass, vk::PipelineBindPoint _bindPoint)
{
	_buffer.bindPipeline(_bindPoint, m_pipelines[_renderPass]);
}
