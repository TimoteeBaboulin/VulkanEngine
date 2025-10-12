#include "Renderer/BufferDeviceLink.h"
#include "Renderer/RendererDeviceManager.h"
#include "Renderer/VulkanHelperFunctions.h"
#include "Renderer/MaterialInstance.h"
#include "Renderer/DrawBuffer.h"

#include "ResourceManagement/TextureData.h"

BufferDeviceLink::BufferDeviceLink(DeviceData _deviceData, MaterialInstance* _materialInstance)
{
	m_deviceData = _deviceData;
	m_material = _materialInstance;

	vk::CommandPoolCreateInfo poolInfo;
	poolInfo.sType = vk::StructureType::eCommandPoolCreateInfo;
	poolInfo.queueFamilyIndex = _deviceData.QueueIndices.graphicsFamily.value();
	poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
	m_commandPool = m_deviceData.Device.createCommandPool(poolInfo);

	vk::CommandBufferAllocateInfo allocInfo;
	allocInfo.sType = vk::StructureType::eCommandBufferAllocateInfo;
	allocInfo.commandPool = m_commandPool;
	allocInfo.level = vk::CommandBufferLevel::ePrimary;
	allocInfo.commandBufferCount = 1;
	auto result = m_deviceData.Device.allocateCommandBuffers(allocInfo);
	m_commandBuffer = result.at(0);

	vk::DescriptorSetAllocateInfo textureSetInfo;
	textureSetInfo.sType = vk::StructureType::eDescriptorSetAllocateInfo;
	textureSetInfo.descriptorPool = m_material->GetDescriptorPool();
	textureSetInfo.descriptorSetCount = 1;
	textureSetInfo.pSetLayouts = &m_material->GetDescriptorSetLayouts()[1];

	m_drawResources.textureDescriptorSets = m_deviceData.Device.allocateDescriptorSets(textureSetInfo);
}

BufferDeviceLink::~BufferDeviceLink()
{
	//TODO: See why I can't destroy the command pool here
	ClearBuffers();
	ClearTextures();
	m_deviceData.Device.freeCommandBuffers(m_commandPool, m_commandBuffer);
	//m_deviceData.Device.destroyCommandPool(m_commandPool);
}

void BufferDeviceLink::Render(vk::CommandBuffer& _cmd, int _renderPass,
	std::vector<MeshEntry>& _meshEntries, vk::DescriptorSet* _uboSet)
{
	vk::DeviceSize offsets[] = { 0 };

	_cmd.bindVertexBuffers(0, m_drawResources.vertexBuffer, offsets);
	_cmd.bindVertexBuffers(1, m_drawResources.modelMatrixBuffer, offsets);
	_cmd.bindIndexBuffer(m_drawResources.indexBuffer, 0, vk::IndexType::eUint16);

	m_material->RecordCommandBuffer(_cmd, _renderPass, vk::PipelineBindPoint::eGraphics);

	_cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_material->GetLayouts()[0], 0, 1, _uboSet, 0, nullptr);
	_cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_material->GetLayouts()[0], 1, 1, m_drawResources.textureDescriptorSets.data(), 0, nullptr);

	int currIndex = 0;
	int currInstance = 0;

	for (int i = 0; i < _meshEntries.size(); i++)
	{
		int instanceCount = _meshEntries[i].ModelMatrices.size();
		int indexCount = _meshEntries[i].Data->triangleCount * 3;

		_cmd.drawIndexed(indexCount, instanceCount, currIndex, 0, currInstance);

		currInstance += instanceCount;
		currIndex += indexCount;
	}
}

void BufferDeviceLink::GenerateBuffers(Vertex* _vertexData, uint32_t _vertexCount,
	uint16_t* _indexData, uint32_t _indexCount, 
	glm::mat4x4* _modelData, uint32_t _modelCount,
	int* _textureIndices)
{
	if (_modelCount == 0)
	{
		return;
	}

	if (m_resourcesGenerated)
	{
		ClearBuffers();
	}

	size_t instanceDataSize = (sizeof(glm::mat4x4) + sizeof(int32_t));
	size_t totalInstanceDataSize = instanceDataSize * _modelCount;
	void* instanceData = new char[totalInstanceDataSize];

	for (int i = 0; i < _modelCount; i++)
	{
		memcpy((char*)instanceData + i * instanceDataSize, &_modelData[i], sizeof(glm::mat4x4));
		// While the texture data from draw buffers assume each mesh instance has 4 textures
		// This is meant to work with my current shader which only uses a single texture
		// TODO: Update this when material can read the texture requirements of the shader
		memcpy((char*)instanceData + i * instanceDataSize + sizeof(glm::mat4x4), &_textureIndices[i], sizeof(int32_t));
	}

	//Create the buffers
	BufferCreateInfo vertexBufferInfo
	{
		.buffer = m_drawResources.vertexBuffer,
		.memory = m_drawResources.vertexBufferMemory,
		.usage = vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
		.properties = vk::MemoryPropertyFlagBits::eDeviceLocal,
		.sharingMode = vk::SharingMode::eExclusive,
		.size = sizeof(Vertex) * _vertexCount
	};
	vhf::CreateBufferWithStaging(m_deviceData.Device, m_deviceData.PhysicalDevice, m_commandPool,
		m_deviceData.Queues.graphicsQueue, vertexBufferInfo, _vertexData);

	BufferCreateInfo indexBufferInfo
	{
		.buffer = m_drawResources.indexBuffer,
		.memory = m_drawResources.indexBufferMemory,
		.usage = vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst,
		.properties = vk::MemoryPropertyFlagBits::eDeviceLocal,
		.sharingMode = vk::SharingMode::eExclusive,
		.size = sizeof(uint16_t) * _indexCount
	};
	vhf::CreateBufferWithStaging(m_deviceData.Device, m_deviceData.PhysicalDevice, m_commandPool,
		m_deviceData.Queues.graphicsQueue, indexBufferInfo, _indexData);

	BufferCreateInfo modelMatrixBufferInfo
	{
		.buffer = m_drawResources.modelMatrixBuffer,
		.memory = m_drawResources.modelMatrixBufferMemory,
		.usage = vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
		.properties = vk::MemoryPropertyFlagBits::eDeviceLocal,
		.sharingMode = vk::SharingMode::eExclusive,
		.size = totalInstanceDataSize
	};
	vhf::CreateBufferWithStaging(m_deviceData.Device, m_deviceData.PhysicalDevice, m_commandPool,
		m_deviceData.Queues.graphicsQueue, modelMatrixBufferInfo, instanceData);

	IsDirty = false;
}

void BufferDeviceLink::GenerateTextures(std::vector<std::shared_ptr<Image>>& _textures)
{
	for (auto it = _textures.begin(); it != _textures.end(); it++)
	{
		auto loadedIt = std::find(m_loadedImages.begin(), m_loadedImages.end(), (*it));
		if (loadedIt == m_loadedImages.end())
		{
			//Need to load the texture
			TextureData texData = GetTextureData(*(*it).get());
			m_drawResources.textures.push_back(texData);
			m_loadedImages.push_back(*it);
		}
	}

	UpdateTextures();
}

void BufferDeviceLink::ClearBuffers()
{
	vk::Device device = m_deviceData.Device;
	device.waitIdle();

	//Free the memory
	device.freeMemory(m_drawResources.vertexBufferMemory);
	device.freeMemory(m_drawResources.indexBufferMemory);
	device.freeMemory(m_drawResources.modelMatrixBufferMemory);

	//Destroy the buffers
	device.destroyBuffer(m_drawResources.vertexBuffer);
	device.destroyBuffer(m_drawResources.indexBuffer);
	device.destroyBuffer(m_drawResources.modelMatrixBuffer);
}

void BufferDeviceLink::ClearTextures()
{
	for (auto it = m_drawResources.textures.begin(); it != m_drawResources.textures.end(); it++)
	{
		m_deviceData.Device.destroyImageView((*it).m_imageView);
		m_deviceData.Device.destroyImage((*it).m_image);
		m_deviceData.Device.destroySampler((*it).m_sampler);
	}
}

TextureData BufferDeviceLink::GetTextureData(Image& _image)
{
	vk::Buffer stagingBuffer;
	vk::DeviceMemory stagingMemory;
	vk::DeviceMemory imageMemory;
	size_t memorySize = _image.width * _image.height * 4;
	vk::Extent2D extent = { _image.width, _image.height };

	TextureData texData;

	//Create staging buffer
	BufferCreateInfo staging = {
		.buffer = stagingBuffer,
		.memory = stagingMemory,
		.usage = vk::BufferUsageFlagBits::eTransferSrc,
		.properties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
		.size = memorySize
	};

	vhf::CreateBuffer(m_deviceData.Device, m_deviceData.PhysicalDevice, staging);
	void* map = m_deviceData.Device.mapMemory(stagingMemory, 0, memorySize);
	memcpy(map, _image.pixels, memorySize);

	//Create Image
	vhf::CreateImage(m_deviceData.Device, m_deviceData.PhysicalDevice, extent, vk::Format::eR8G8B8A8Srgb, vk::ImageTiling::eOptimal,
		vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal,
		texData.m_image, imageMemory, vk::ImageLayout::eUndefined);

	TransitionInfo transInfo =
	{
		.srcAccessFlags = vk::AccessFlagBits::eNone,
		.dstAccessFlags = vk::AccessFlagBits::eTransferWrite,
		.srcStage = vk::PipelineStageFlagBits::eTopOfPipe,
		.dstStage = vk::PipelineStageFlagBits::eTransfer
	};
	vhf::TransitionImageLayout(m_deviceData.Device, m_commandPool, m_deviceData.Queues.graphicsQueue, texData.m_image, vk::Format::eR8G8B8A8Srgb, vk::ImageAspectFlagBits::eColor, vk::ImageLayout::eUndefined,
		vk::ImageLayout::eTransferDstOptimal, transInfo);

	//Copy staging buffer to image
	vhf::CopyBufferToImage(m_deviceData.Device, m_commandPool, m_deviceData.Queues.graphicsQueue, stagingBuffer, texData.m_image, extent);

	//Prepare for shader read
	transInfo =
	{
		.srcAccessFlags = vk::AccessFlagBits::eTransferWrite,
		.dstAccessFlags = vk::AccessFlagBits::eShaderRead,
		.srcStage = vk::PipelineStageFlagBits::eTransfer,
		.dstStage = vk::PipelineStageFlagBits::eFragmentShader
	};

	vhf::TransitionImageLayout(m_deviceData.Device, m_commandPool, m_deviceData.Queues.graphicsQueue, texData.m_image, vk::Format::eR8G8B8A8Srgb, vk::ImageAspectFlagBits::eColor, vk::ImageLayout::eTransferDstOptimal,
		vk::ImageLayout::eShaderReadOnlyOptimal, transInfo);

	//std::vector<vk::ImageView> imageViews(1);
	texData.m_imageView = vhf::CreateImageView(m_deviceData.Device, texData.m_image, vk::Format::eR8G8B8A8Srgb, vk::ImageAspectFlagBits::eColor);

	//std::vector<vk::Sampler> samplers(1);

	//Create sampler
	vk::SamplerCreateInfo info;
	info.sType = vk::StructureType::eSamplerCreateInfo;
	info.magFilter = vk::Filter::eLinear;
	info.minFilter = vk::Filter::eLinear;

	info.addressModeU = vk::SamplerAddressMode::eRepeat;
	info.addressModeV = vk::SamplerAddressMode::eRepeat;
	info.addressModeW = vk::SamplerAddressMode::eRepeat;

	info.anisotropyEnable = false;
	info.maxAnisotropy = 4;
	info.borderColor = vk::BorderColor::eIntOpaqueBlack;
	info.unnormalizedCoordinates = false;

	info.compareEnable = false;
	info.compareOp = vk::CompareOp::eAlways;

	info.mipmapMode = vk::SamplerMipmapMode::eLinear;
	info.mipLodBias = 0.0f;
	info.minLod = 0.0f;
	info.maxLod = 0.0f;

	texData.m_sampler = m_deviceData.Device.createSampler(info);

	//Free staging buffer
	m_deviceData.Device.unmapMemory(stagingMemory);
	m_deviceData.Device.destroyBuffer(stagingBuffer);

	return texData;
}

void BufferDeviceLink::UpdateTextures()
{
	size_t textureCount = m_drawResources.textures.size();
	vk::WriteDescriptorSet* writeSets = new vk::WriteDescriptorSet[textureCount];
	vk::DescriptorImageInfo* imageInfos = new vk::DescriptorImageInfo[textureCount];

	for (int i = 0; i < textureCount; i++)
	{
		TextureData& texture = m_drawResources.textures[i];
		imageInfos[i].imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		imageInfos[i].imageView = texture.m_imageView;
		imageInfos[i].sampler = texture.m_sampler;

		writeSets[i].sType = vk::StructureType::eWriteDescriptorSet;
		writeSets[i].dstSet = m_drawResources.textureDescriptorSets[0];
		writeSets[i].dstBinding = 0;
		writeSets[i].dstArrayElement = i;
		writeSets[i].descriptorType = vk::DescriptorType::eCombinedImageSampler;
		writeSets[i].descriptorCount = 1;
		writeSets[i].pImageInfo = &imageInfos[i];
	}

	m_deviceData.Device.updateDescriptorSets(textureCount, writeSets, 0, nullptr);
}