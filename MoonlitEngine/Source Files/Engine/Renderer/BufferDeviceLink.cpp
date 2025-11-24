#include "Engine/Renderer/BufferDeviceLink.h"
#include "Engine/Renderer/VulkanHelperFunctions.h"
#include "Engine/Renderer/Material/MaterialInstance.h"
#include "Engine/Renderer/DrawBuffer.h"

#include "ResourceManagement/TextureData.h"

BufferDeviceLink::BufferDeviceLink(DeviceData _deviceData, std::shared_ptr<MaterialInstance> _materialInstance,
	DrawBuffer* _drawBuffer) : m_parentBuffer(_drawBuffer), m_materialInstance(_materialInstance)
{
	m_deviceData = _deviceData;

	AllocateCommandBuffer();
	AllocateTextureSets();

	// Initial data upload
	UpdateData();
}

BufferDeviceLink::BufferDeviceLink(BufferDeviceLink&& _src)
{
	// We can't keep the source's resources, not only is it bad practice
	// But using the same resources from multiple places will cause vulkan crashes that are hell to debug
	m_deviceData = _src.m_deviceData;
	m_materialInstance = _src.m_materialInstance;
	m_commandPool = _src.m_commandPool;
	m_commandBuffer = _src.m_commandBuffer;
	m_drawResources = _src.m_drawResources;
	_src.m_materialInstance = nullptr;
	_src.m_commandBuffer = nullptr;
	_src.m_commandPool = nullptr;
	_src.m_deviceData = {};
	_src.m_drawResources = {};
}

BufferDeviceLink& BufferDeviceLink::operator=(BufferDeviceLink&& _rhs)
{
	// We can't keep the source's resources, not only is it bad practice
	// But using the same resources from multiple places will cause vulkan crashes that are hell to debug

	if (this != &_rhs)
	{
		m_deviceData = _rhs.m_deviceData;
		m_materialInstance = _rhs.m_materialInstance;
		m_commandPool = _rhs.m_commandPool;
		m_commandBuffer = _rhs.m_commandBuffer;
		m_drawResources = _rhs.m_drawResources;
		_rhs.m_materialInstance = nullptr;
		_rhs.m_commandBuffer = nullptr;
		_rhs.m_commandPool = nullptr;
		_rhs.m_deviceData = {};
		_rhs.m_drawResources = {};
	}

	return *this;
}

BufferDeviceLink::~BufferDeviceLink()
{
	ClearBuffers();
	ClearTextures();
	m_deviceData.Device.freeCommandBuffers(m_commandPool, m_commandBuffer);
	m_deviceData.Device.destroyCommandPool(m_commandPool);
}

void BufferDeviceLink::Render(vk::CommandBuffer& _cmd, std::string _renderPass,
	vk::DescriptorSet* _uboSet)
{
	if (m_isDirty)
	{
		UpdateData();
	}

	vk::DeviceSize offsets[] = { 0 };

	_cmd.bindVertexBuffers(0, m_drawResources.vertexBuffer, offsets);
	_cmd.bindVertexBuffers(1, m_drawResources.modelMatrixBuffer, offsets);
	_cmd.bindIndexBuffer(m_drawResources.indexBuffer, 0, vk::IndexType::eUint16);

	m_materialInstance->RecordCommandBuffer(_cmd, _renderPass, vk::PipelineBindPoint::eGraphics);

	_cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_materialInstance->GetLayouts()[0], 0, 1, _uboSet, 0, nullptr);
	_cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_materialInstance->GetLayouts()[0], 1, 1, m_drawResources.textureDescriptorSets.data(), 0, nullptr);

	int currIndex = 0;
	int currInstance = 0;

	auto meshEntries = m_parentBuffer->GetMeshEntries();

	for (int i = 0; i < meshEntries.size(); i++)
	{
		size_t instanceCount = meshEntries[i].InstanceCount;
		int indexCount = meshEntries[i].Data.triangleCount * 3;

		_cmd.drawIndexed(indexCount, (uint32_t) instanceCount, currIndex, 0, currInstance);

		currInstance += (int)instanceCount;
		currIndex += indexCount;
	}
}

void BufferDeviceLink::SetDirty()
{
	m_isDirty = true;
}

// Helper functions

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
		.Size = memorySize
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

	texData.m_imageView = vhf::CreateImageView(m_deviceData.Device, texData.m_image, vk::Format::eR8G8B8A8Srgb, vk::ImageAspectFlagBits::eColor);

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

// Update functions

void BufferDeviceLink::UpdateData()
{
	UpdateBuffers();
	UpdateTextures();
}

void BufferDeviceLink::UpdateBuffers()
{
	// Make sure to clear previous buffers if they exist
	if (m_resourcesGenerated)
		ClearBuffers();

	GenerateBuffers();
}

void BufferDeviceLink::UpdateTextures()
{
	std::vector<std::shared_ptr<Image>> textures = m_parentBuffer->GetAllTextures();
	//Load the textures to the device
	GenerateTextures(textures);
	//Write the descriptor set to hold the textures
	UpdateTextureSets();
}

void BufferDeviceLink::UpdateTextureSets()
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

	m_deviceData.Device.updateDescriptorSets((uint32_t)textureCount, writeSets, 0, nullptr);
}

// Memory management functions

void BufferDeviceLink::AllocateCommandBuffer()
{
	vk::CommandPoolCreateInfo poolInfo;
	poolInfo.sType = vk::StructureType::eCommandPoolCreateInfo;
	poolInfo.queueFamilyIndex = m_deviceData.QueueIndices.graphicsFamily.value();
	poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
	m_commandPool = m_deviceData.Device.createCommandPool(poolInfo);

	vk::CommandBufferAllocateInfo allocInfo;
	allocInfo.sType = vk::StructureType::eCommandBufferAllocateInfo;
	allocInfo.commandPool = m_commandPool;
	allocInfo.level = vk::CommandBufferLevel::ePrimary;
	allocInfo.commandBufferCount = 1;
	auto result = m_deviceData.Device.allocateCommandBuffers(allocInfo);
	m_commandBuffer = result.at(0);
}

void BufferDeviceLink::AllocateTextureSets()
{
	vk::DescriptorSetAllocateInfo textureSetInfo;
	textureSetInfo.sType = vk::StructureType::eDescriptorSetAllocateInfo;
	textureSetInfo.descriptorPool = m_materialInstance->GetDescriptorPool();
	textureSetInfo.descriptorSetCount = 1;
	textureSetInfo.pSetLayouts = &m_materialInstance->GetDescriptorSetLayouts()[1];

	m_drawResources.textureDescriptorSets = m_deviceData.Device.allocateDescriptorSets(textureSetInfo);
}

void BufferDeviceLink::GenerateBuffers()
{
	// Copy the data from the parent buffer
	std::vector<Vertex> vertexData = m_parentBuffer->GetVertexData();
	std::vector<uint16_t> indexData = m_parentBuffer->GetIndexData();
	std::vector<glm::mat4x4> modelData = m_parentBuffer->GetModelData();
	std::vector<uint32_t> textureIndices = m_parentBuffer->GetTextureIndices();

	uint32_t vertexCount = (uint32_t)vertexData.size();
	uint32_t indexCount = (uint32_t)indexData.size();
	uint32_t modelCount = (uint32_t)modelData.size();

	// If there's no data, we don't create the buffers
	// So we need to be very wary about continuing the render loop if this happens
	// If there's no models, the DrawBuffer should be deleted to clear up resources
	if (modelCount == 0)
		return;

	size_t textureCount = m_materialInstance->GetTextureCount();
	size_t instanceDataSize = sizeof(glm::mat4x4) + sizeof(uint32_t) * textureCount;
	size_t totalInstanceDataSize = instanceDataSize * modelCount;
	void* instanceData = new char[totalInstanceDataSize];

	for (uint32_t i = 0; i < modelCount; i++)
	{
		memcpy((char*)instanceData + i * instanceDataSize, &modelData[i], sizeof(glm::mat4x4));
		memcpy((char*)instanceData + i * instanceDataSize + sizeof(glm::mat4x4), &textureIndices[i], sizeof(uint32_t) * textureCount);
	}

	//Create the buffers
	BufferCreateInfo vertexBufferInfo
	{
		.buffer = m_drawResources.vertexBuffer,
		.memory = m_drawResources.vertexBufferMemory,
		.usage = vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
		.properties = vk::MemoryPropertyFlagBits::eDeviceLocal,
		.sharingMode = vk::SharingMode::eExclusive,
		.Size = sizeof(Vertex) * vertexCount
	};
	vhf::CreateBufferWithStaging(m_deviceData.Device, m_deviceData.PhysicalDevice, m_commandPool,
		m_deviceData.Queues.graphicsQueue, vertexBufferInfo, vertexData.data());

	BufferCreateInfo indexBufferInfo
	{
		.buffer = m_drawResources.indexBuffer,
		.memory = m_drawResources.indexBufferMemory,
		.usage = vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst,
		.properties = vk::MemoryPropertyFlagBits::eDeviceLocal,
		.sharingMode = vk::SharingMode::eExclusive,
		.Size = sizeof(uint16_t) * indexCount
	};
	vhf::CreateBufferWithStaging(m_deviceData.Device, m_deviceData.PhysicalDevice, m_commandPool,
		m_deviceData.Queues.graphicsQueue, indexBufferInfo, indexData.data());

	BufferCreateInfo modelMatrixBufferInfo
	{
		.buffer = m_drawResources.modelMatrixBuffer,
		.memory = m_drawResources.modelMatrixBufferMemory,
		.usage = vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
		.properties = vk::MemoryPropertyFlagBits::eDeviceLocal,
		.sharingMode = vk::SharingMode::eExclusive,
		.Size = totalInstanceDataSize
	};
	vhf::CreateBufferWithStaging(m_deviceData.Device, m_deviceData.PhysicalDevice, m_commandPool,
		m_deviceData.Queues.graphicsQueue, modelMatrixBufferInfo, instanceData);

	m_isDirty = false;
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

	UpdateTextureSets();
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

