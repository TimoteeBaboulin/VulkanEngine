#include "MoonlitVulkan/VulkanData.h"
#include "MoonlitVulkan/VulkanHelperFunctions.h"

void Mesh::Load(vk::Device device, MeshData _data, vk::DescriptorSetLayout _layout, vk::DescriptorPool _pool)
{
	m_vertexCount = (MeshCountType)_data.vertexCount;
	m_triangleCount = (MeshCountType)_data.triangleCount;

	if (m_vertexCount == 0)
	{
		throw new std::exception("No vertices to load");
	}

	if (m_triangleCount == 0)
	{
		throw new std::exception("No triangles to load");
	}

	uint64_t vertexBufferSize = sizeof(Vertex) * _data.vertexCount;

	VertexBufferInfo bufferInfo = {
		.buffer = m_vertexBuffer,
		.memory = m_vertexMemory,
		.usage = vk::BufferUsageFlagBits::eVertexBuffer,
		.properties = vk::MemoryPropertyFlagBits::eDeviceLocal,
		.size = vertexBufferSize
	};

	vhf::CreateBufferWithStaging(bufferInfo, _data.vertices);

	uint64_t indexBufferSize = sizeof(uint32_t) * _data.triangleCount * 3;

	VertexBufferInfo indexBufferInfo = {
		.buffer = m_indexBuffer,
		.memory = m_indexMemory,
		.usage = vk::BufferUsageFlagBits::eIndexBuffer,
		.properties = vk::MemoryPropertyFlagBits::eDeviceLocal,
		.size = indexBufferSize
	};

	vhf::CreateBufferWithStaging(indexBufferInfo, _data.indices);

	m_textureCount = (uint16_t)_data.textures.size();
	m_textureImages.resize(m_textureCount);
	m_textureImageViews.resize(m_textureCount);
	m_textureSamplers.resize(m_textureCount);

	int index = 0;

	for (Image& image : _data.textures)
	{
		vk::Buffer stagingBuffer;
		vk::DeviceMemory stagingMemory;
		vk::DeviceMemory imageMemory;
		size_t memorySize = image.width * image.height * 4;
		vk::Extent2D extent = { image.width, image.height };

		//Create staging buffer
		VertexBufferInfo staging = {
			.buffer = stagingBuffer,
			.memory = stagingMemory,
			.usage = vk::BufferUsageFlagBits::eTransferSrc,
			.properties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
			.size = memorySize
		};

		vhf::CreateBuffer(staging);
		void* map = device.mapMemory(stagingMemory, 0, memorySize);
		memcpy(map, image.pixels, memorySize);

		//Create Image
		vhf::CreateImage(extent, vk::Format::eR8G8B8A8Srgb, vk::ImageTiling::eOptimal,
			vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal,
			m_textureImages[index], imageMemory, vk::ImageLayout::eUndefined);

		TransitionInfo transInfo =
		{
			.srcAccessFlags = vk::AccessFlagBits::eNone,
			.dstAccessFlags = vk::AccessFlagBits::eTransferWrite,
			.srcStage = vk::PipelineStageFlagBits::eTopOfPipe,
			.dstStage = vk::PipelineStageFlagBits::eTransfer
		};
		vhf::TransitionImageLayout(m_textureImages[index], vk::Format::eR8G8B8A8Srgb, vk::ImageAspectFlagBits::eColor, vk::ImageLayout::eUndefined,
			vk::ImageLayout::eTransferDstOptimal, transInfo);

		//Copy staging buffer to image
		vhf::CopyBufferToImage(stagingBuffer, m_textureImages[index], extent);

		//Prepare for shader read
		transInfo =
		{
			.srcAccessFlags = vk::AccessFlagBits::eTransferWrite,
			.dstAccessFlags = vk::AccessFlagBits::eShaderRead,
			.srcStage = vk::PipelineStageFlagBits::eTransfer,
			.dstStage = vk::PipelineStageFlagBits::eFragmentShader
		};

		vhf::TransitionImageLayout(m_textureImages[index], vk::Format::eR8G8B8A8Srgb, vk::ImageAspectFlagBits::eColor, vk::ImageLayout::eTransferDstOptimal,
			vk::ImageLayout::eShaderReadOnlyOptimal, transInfo);

		m_textureImageViews[index] = vhf::CreateImageView(m_textureImages[index], vk::Format::eR8G8B8A8Srgb, vk::ImageAspectFlagBits::eColor);

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

		m_textureSamplers[index] = VulkanEngine::LogicalDevice.createSampler(info);

		//Free staging buffer
		index++;
		device.unmapMemory(stagingMemory);
		device.destroyBuffer(stagingBuffer);
	}

	m_materials.resize(1);
	m_materials[0].AllocateSets(device, _layout, _pool, m_textureCount);
	m_materials[0].UpdateSets(device, m_textureImageViews, m_textureSamplers);
}

void Mesh::BindSets(vk::CommandBuffer _buffer, vk::PipelineLayout _layout)
{
	m_materials[0].BindSets(_buffer, _layout);
}

void Material::AllocateSets(vk::Device _device, vk::DescriptorSetLayout _descriptorLayout, vk::DescriptorPool _descriptorPool, int _count)
{
	m_descriptorSets.resize(_count);

	vk::DescriptorSetAllocateInfo allocInfo;
	allocInfo.sType = vk::StructureType::eDescriptorSetAllocateInfo;
	allocInfo.descriptorPool = _descriptorPool;
	allocInfo.descriptorSetCount = _count;
	allocInfo.pSetLayouts = &_descriptorLayout;
	
	_device.allocateDescriptorSets(allocInfo, m_descriptorSets.data());
}

void Material::UpdateSets(vk::Device _device, std::vector<vk::ImageView> _views, std::vector<vk::Sampler> _samplers)
{
	std::vector<vk::WriteDescriptorSet> writeSets(m_descriptorSets.size());

	for (int i = 0; i < m_descriptorSets.size(); i++)
	{
		vk::DescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		imageInfo.imageView = _views[i];
		imageInfo.sampler = _samplers[i];

		vk::WriteDescriptorSet combinedSamplerSet;
		combinedSamplerSet.sType = vk::StructureType::eWriteDescriptorSet;
		combinedSamplerSet.dstSet = m_descriptorSets[i];
		combinedSamplerSet.dstBinding = 1;
		combinedSamplerSet.dstArrayElement = 0;
		combinedSamplerSet.descriptorType = vk::DescriptorType::eCombinedImageSampler;
		combinedSamplerSet.descriptorCount = 1;
		combinedSamplerSet.pImageInfo = &imageInfo;
	}

	_device.updateDescriptorSets(writeSets.size(), writeSets.data(), 0, nullptr);
}

void Material::BindSets(vk::CommandBuffer _buffer, vk::PipelineLayout _layout)
{
	_buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, _layout, 1, m_descriptorSetCount, m_descriptorSets.data(), 0, nullptr);
}
