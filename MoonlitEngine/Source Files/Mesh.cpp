#include "Mesh.h"
#include "Renderer/VulkanHelperFunctions.h"

void Mesh::Load(vk::Device device, MeshData _data, vk::DescriptorSetLayout _layout, vk::DescriptorPool _pool)
{
	m_modelMatrixes = std::vector<glm::mat4>();
	m_descriptorPool = _pool;

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

	//m_materials.resize(1);
	//m_materials[0]->AllocateSets(device, _layout, _pool, m_textureCount);
	//m_materials[0]->UpdateSets(device, m_textureImageViews, m_textureSamplers);
}

void Mesh::SetMaterials(MaterialInstance** _pInstances, size_t _instancesCount, vk::Device _device)
{
	m_materials.resize(_instancesCount);

	for (size_t i = 0; i < _instancesCount; i++)
	{
		m_materials[i] = _pInstances[i];
		m_materials[i]->UpdateSets(_device, m_textureImageViews, m_textureSamplers);

	}
}

void Mesh::CleanUp(vk::Device _device)
{
	for (int i = 0; i < m_textureCount; i++)
	{
		m_materials[i]->CleanUp(_device, m_descriptorPool);

		_device.destroySampler(m_textureSamplers[i]);
		_device.destroyImageView(m_textureImageViews[i]);
		_device.destroyImage(m_textureImages[i]);
	}

	_device.unmapMemory(m_indexMemory);
	_device.unmapMemory(m_vertexMemory);

	_device.destroyBuffer(m_indexBuffer);
	_device.destroyBuffer(m_vertexBuffer);
}

//void Mesh::BindSets(vk::CommandBuffer _buffer, vk::PipelineLayout _layout)
//{
//	m_materials[0]->BindSets(_buffer, _layout);
//}

void Mesh::RecordCommandBuffer(vk::CommandBuffer _buffer, int _renderPass, vk::PipelineBindPoint _bindPoint)
{
	vk::DeviceSize offsets[] = { 0 };


	for (auto& material : m_materials)
	{
		material->RecordCommandBuffer(_buffer, _renderPass, _bindPoint);
		_buffer.bindVertexBuffers(0, m_vertexBuffer, offsets);
		_buffer.bindIndexBuffer(m_indexBuffer, 0, vk::IndexType::eUint32);
		_buffer.drawIndexed(m_triangleCount * 3, 1, 0, 0, 0);
	}
}

bool operator==(const MeshInstance& _a, const MeshInstance& _b)
{
	return _a.m_material == _b.m_material && _a.m_mesh == _b.m_mesh && _a.m_materialInstance == _b.m_materialInstance;
}
