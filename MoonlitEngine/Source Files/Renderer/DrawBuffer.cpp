#include "Renderer/DrawBuffer.h"
#include "Renderer/Renderer.h"
#include "Renderer/BufferDeviceLink.h"
#include "Renderer/RenderTarget.h"

//DrawBuffer::DrawBuffer(Material* _material, Renderer& _renderer) : m_material(_material)
//{
//	m_vertexData = new Vertex[MaxVertexCount];
//	m_indexData = new uint16_t[MaxIndexCount];
//	m_modelData = new glm::mat4x4[MaxModelCount];
//
//	m_dirty = false;
//
//	m_vertexCount = 0;
//	m_indexCount = 0;
//	m_instanceCount = 0;
//	
//	//Create a descriptor pool that can keep as many textures as our array size and up to 10 uniform buffers (camera data)
//	vk::DescriptorPoolSize* sizes = new vk::DescriptorPoolSize[1];
//	vk::DescriptorPoolCreateInfo poolInfo;
//
//	sizes[0].type = vk::DescriptorType::eCombinedImageSampler;
//	sizes[0].descriptorCount = TextureArrayCount;
//
//	poolInfo.sType = vk::StructureType::eDescriptorPoolCreateInfo;
//	poolInfo.poolSizeCount = 1;
//	poolInfo.pPoolSizes = sizes;
//	poolInfo.maxSets = 32;
//
//	m_descriptorPool = m_device.createDescriptorPool(poolInfo);
//}

DrawBuffer::DrawBuffer(Material* _material)
{
	m_material = _material;

	m_vertexData = new Vertex[MaxVertexCount];
	m_indexData = new uint16_t[MaxIndexCount];
	m_modelData = new glm::mat4x4[MaxModelCount];

	m_vertexCount = 0;
	m_indexCount = 0;
	m_instanceCount = 0;
}

bool DrawBuffer::TryAddMesh(MeshInstance* _instance)
{
	if (std::find(m_meshInstances.begin(), m_meshInstances.end(), _instance) != m_meshInstances.end())
	{
		//TODO: Log a warning

		return false;
	}

	m_meshInstances.push_back(_instance);

	for (auto dit = m_deviceLinks.begin(); dit != m_deviceLinks.end(); dit++)
	{
		(*dit).IsDirty = true;
	}

	UpdateEntries();

	return true;
}

void DrawBuffer::RemoveMesh(MeshInstance* _instance)
{
	auto it = std::find(m_meshInstances.begin(), m_meshInstances.end(), _instance);
	if (it != m_meshInstances.end())
		m_meshInstances.erase(it);

	for (auto dit = m_deviceLinks.begin(); dit != m_deviceLinks.end(); dit++)
	{
		(*dit).IsDirty = true;
	}

	UpdateEntries();
}

void DrawBuffer::LinkTarget(RenderTarget& _renderTarget)
{
	auto it = FindDeviceLink(_renderTarget);
	if (it != m_deviceLinks.end())
	{
		//TODO: Log a warning
		return;
	}

	m_deviceLinks.push_back(BufferDeviceLink(_renderTarget.GetDeviceData()));
	m_deviceLinks.back().GenerateBuffers(m_vertexData, m_vertexCount,
		m_indexData, m_indexCount,
		m_modelData, m_instanceCount);
}

void DrawBuffer::UpdateBuffers()
{
	CountVertexData();

	Vertex* vertexDataPtr = m_vertexData;
	uint16_t* indexDataPtr = m_indexData;
	glm::mat4x4* modelDataPtr = m_modelData;

	//Go over all the entries as they are meant to give an easy way to write the data as it comes
	for (auto it = m_meshes.begin(); it != m_meshes.end(); it++)
	{
		MeshData* data = (*it).Data;
		memcpy(vertexDataPtr, data->vertices, sizeof(Vertex) * data->vertexCount);
		vertexDataPtr = vertexDataPtr + data->vertexCount;

		memcpy(indexDataPtr, data->indices, sizeof(uint16_t) * data->triangleCount * 3);
		indexDataPtr = indexDataPtr + (data->triangleCount * 3);

		for (auto mit = (*it).ModelMatrices.begin(); mit != (*it).ModelMatrices.end(); mit++)
		{
			*modelDataPtr = (*mit);
			modelDataPtr++;
		}
	}
}

void DrawBuffer::RenderBuffer(RenderTarget& _target, vk::CommandBuffer& _cmd, int _renderPass)
{
	auto it = FindDeviceLink(_target);
	if ((*it).IsDirty)
	{
		(*it).GenerateBuffers(m_vertexData, m_vertexCount,
			m_indexData, m_indexCount,
			m_modelData, m_instanceCount);
	}
}

//void DrawBuffer::UpdateTextures(int _deviceIndex)
//{
//	if (!m_texturesDirty)
//		return;
//
//	if (_deviceIndex >= m_linkedDevices.size())
//		throw new std::exception("Device index out of range");
//
//	size_t textureCount = m_textures.size();
//	vk::WriteDescriptorSet* writeSets = new vk::WriteDescriptorSet[textureCount];
//	vk::DescriptorImageInfo* imageInfos = new vk::DescriptorImageInfo[textureCount];
//
//	for (int i = 0; i < textureCount; i++)
//	{
//		imageInfos[i].imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
//		imageInfos[i].imageView = m_textures[i].m_imageView;
//		imageInfos[i].sampler = m_textures[i].m_sampler;
//
//		writeSets[i].sType = vk::StructureType::eWriteDescriptorSet;
//		writeSets[i].dstSet = m_descriptorSets[0];
//		writeSets[i].dstBinding = 0;
//		writeSets[i].dstArrayElement = i;
//		writeSets[i].descriptorType = vk::DescriptorType::eCombinedImageSampler;
//		writeSets[i].descriptorCount = 1;
//		writeSets[i].pImageInfo = &imageInfos[i];
//	}
//
//	m_device.updateDescriptorSets(textureCount, writeSets, 0, nullptr);
//
//	m_texturesDirty = false;
//}
//
//void DrawBuffer::RenderBuffer(vk::CommandBuffer _cmd, vk::DescriptorSet* _uboSet, int _currentPass)
//{
//	if (!m_buffersGenerated || m_dirty)
//	{
//		for (int i = 0; i < m_linkedDevices.size(); i++)
//		{
//			UpdateBuffers(i);
//		}
//	}
//
//	vk::DeviceSize offsets[] = { 0 };
//
//	_cmd.bindVertexBuffers(0, m_vertexBuffer, offsets);
//	_cmd.bindVertexBuffers(1, m_modelMatriceBuffer, offsets);
//	_cmd.bindIndexBuffer(m_indexBuffer, 0, vk::IndexType::eUint16);
//
//	m_material->RecordCommandBuffer(_cmd, _currentPass, vk::PipelineBindPoint::eGraphics);
//
//	_cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_material->GetLayouts()[0], 0, 1, _uboSet, 0, nullptr);
//	_cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_material->GetLayouts()[0], 1, 1, m_descriptorSets.data(), 0, nullptr);
//
//	int currIndex = 0;
//	int currInstance = 0;
//
//	for (int i = 0; i < m_meshes.size(); i++)
//	{
//		int instanceCount = m_meshInstanceCount[i];
//		int indexCount = m_meshes[i]->triangleCount * 3;
//
//		_cmd.drawIndexed(indexCount, instanceCount, currIndex, 0, currInstance);
//
//		currInstance += instanceCount;
//		currIndex += indexCount;
//	}
//}

//void DrawBuffer::RenderBufferIndirect(vk::CommandBuffer _cmd, vk::DescriptorSet* _uboSet, int _currentPass)
//{
//	if (!m_buffersGenerated || m_dirty)
//		return;
//
//	vk::DeviceSize offsets[] = { 0 };
//
//	_cmd.bindVertexBuffers(0, m_vertexBuffer, offsets);
//	_cmd.bindVertexBuffers(1, m_modelMatriceBuffer, offsets);
//	_cmd.bindIndexBuffer(m_indexBuffer, 0, vk::IndexType::eUint16);
//
//	m_material->RecordCommandBuffer(_cmd, _currentPass, vk::PipelineBindPoint::eGraphics);
//
//	_cmd.drawIndexedIndirect(m_drawCommandBuffer, 0, m_meshes.size(), sizeof(vk::DrawIndexedIndirectCommand));
//}

void DrawBuffer::CountVertexData()
{
	m_vertexCount = 0;
	m_indexCount = 0;
	m_instanceCount = 0;

	for (auto it = m_meshes.begin(); it != m_meshes.end(); it++)
	{
		MeshData* data = (*it).Data;

		int instanceCount = (*it).ModelMatrices.size();
		m_instanceCount += instanceCount;
		m_vertexCount += data->vertexCount;
		m_indexCount += data->triangleCount * 3;
	}
}

void DrawBuffer::UpdateEntries()
{
	m_meshes.clear();

	for (int index = 0; index < m_meshInstances.size(); index++)
	{
		MeshInstance& instance = *m_meshInstances[index];
		MeshData* meshData = &instance.MeshData;
		auto meshEntryIt = m_meshes.end();

		for (auto it = m_meshes.begin(); it != m_meshes.end(); it++)
		{
			if ((*it).Data == meshData)
			{
				meshEntryIt = it;
				break;
			}
		}

		//TODO: Add checks to know if there is enough place in the buffer to add the mesh instance

		if (meshEntryIt == m_meshes.end())
		{
			m_meshes.push_back(MeshEntry{
				meshData,
				{instance.Model}
				});

			continue;
		}

		(*meshEntryIt).ModelMatrices.push_back(instance.Model);
	}
}

std::vector<BufferDeviceLink>::iterator DrawBuffer::FindDeviceLink(RenderTarget& _target)
{
	for (auto it = m_deviceLinks.begin(); it != m_deviceLinks.end(); it++)
	{
		if ((*it).GetDevice() == _target.GetDevice())
		{
			return it;
		}
	}

	return m_deviceLinks.end();
}

//TextureData DrawBuffer::GetTextureData(Image _image)
//{
//	vk::Buffer stagingBuffer;
//	vk::DeviceMemory stagingMemory;
//	vk::DeviceMemory imageMemory;
//	size_t memorySize = _image.width * _image.height * 4;
//	vk::Extent2D extent = { _image.width, _image.height };
//
//	TextureData texData;
//
//	//Create staging buffer
//	BufferCreateInfo staging = {
//		.buffer = stagingBuffer,
//		.memory = stagingMemory,
//		.usage = vk::BufferUsageFlagBits::eTransferSrc,
//		.properties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
//		.size = memorySize
//	};
//
//	vhf::CreateBuffer(m_device, m_physicalDevice, staging);
//	void* map = m_device.mapMemory(stagingMemory, 0, memorySize);
//	memcpy(map, _image.pixels, memorySize);
//
//	//Create Image
//	vhf::CreateImage(m_device, m_physicalDevice, extent, vk::Format::eR8G8B8A8Srgb, vk::ImageTiling::eOptimal,
//		vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal,
//		texData.m_image, imageMemory, vk::ImageLayout::eUndefined);
//
//	TransitionInfo transInfo =
//	{
//		.srcAccessFlags = vk::AccessFlagBits::eNone,
//		.dstAccessFlags = vk::AccessFlagBits::eTransferWrite,
//		.srcStage = vk::PipelineStageFlagBits::eTopOfPipe,
//		.dstStage = vk::PipelineStageFlagBits::eTransfer
//	};
//	vhf::TransitionImageLayout(m_device, m_mainCommandPool, m_graphicsQueue, texData.m_image, vk::Format::eR8G8B8A8Srgb, vk::ImageAspectFlagBits::eColor, vk::ImageLayout::eUndefined,
//		vk::ImageLayout::eTransferDstOptimal, transInfo);
//
//	//Copy staging buffer to image
//	vhf::CopyBufferToImage(m_device, m_mainCommandPool, m_graphicsQueue, stagingBuffer, texData.m_image, extent);
//
//	//Prepare for shader read
//	transInfo =
//	{
//		.srcAccessFlags = vk::AccessFlagBits::eTransferWrite,
//		.dstAccessFlags = vk::AccessFlagBits::eShaderRead,
//		.srcStage = vk::PipelineStageFlagBits::eTransfer,
//		.dstStage = vk::PipelineStageFlagBits::eFragmentShader
//	};
//
//	vhf::TransitionImageLayout(m_device, m_mainCommandPool, m_graphicsQueue, texData.m_image, vk::Format::eR8G8B8A8Srgb, vk::ImageAspectFlagBits::eColor, vk::ImageLayout::eTransferDstOptimal,
//		vk::ImageLayout::eShaderReadOnlyOptimal, transInfo);
//
//	//std::vector<vk::ImageView> imageViews(1);
//	texData.m_imageView = vhf::CreateImageView(m_device, texData.m_image, vk::Format::eR8G8B8A8Srgb, vk::ImageAspectFlagBits::eColor);
//
//	//std::vector<vk::Sampler> samplers(1);
//
//	//Create sampler
//	vk::SamplerCreateInfo info;
//	info.sType = vk::StructureType::eSamplerCreateInfo;
//	info.magFilter = vk::Filter::eLinear;
//	info.minFilter = vk::Filter::eLinear;
//
//	info.addressModeU = vk::SamplerAddressMode::eRepeat;
//	info.addressModeV = vk::SamplerAddressMode::eRepeat;
//	info.addressModeW = vk::SamplerAddressMode::eRepeat;
//
//	info.anisotropyEnable = false;
//	info.maxAnisotropy = 4;
//	info.borderColor = vk::BorderColor::eIntOpaqueBlack;
//	info.unnormalizedCoordinates = false;
//
//	info.compareEnable = false;
//	info.compareOp = vk::CompareOp::eAlways;
//
//	info.mipmapMode = vk::SamplerMipmapMode::eLinear;
//	info.mipLodBias = 0.0f;
//	info.minLod = 0.0f;
//	info.maxLod = 0.0f;
//
//	texData.m_sampler = m_device.createSampler(info);
//
//	//Free staging buffer
//	m_device.unmapMemory(stagingMemory);
//	m_device.destroyBuffer(stagingBuffer);
//
//	return texData;
//}
//
//void DrawBuffer::AddTextures(std::vector<std::shared_ptr<Image>> _images)
//{
//	for (auto it = _images.begin(); it != _images.end(); it++)
//	{
//		auto imageIt = std::find(m_images.begin(), m_images.end(), (*it));
//		if (imageIt == m_images.end())
//		{
//			//Need to add a new texture to the list
//			m_textureIndexes.push_back(m_textures.size());
//			m_images.push_back(*it);
//			m_textures.push_back(GetTextureData(*(*it).get()));
//
//			m_texturesDirty = true;
//		}
//		else
//		{
//			//Texture alread exists
//			m_textureIndexes.push_back(std::distance(m_images.begin(), imageIt));
//		}
//	}
//}
//
//void DrawBuffer::AddDevice(vk::Device _device, vk::PhysicalDevice _physDevice)
//{
//	m_linkedDevices.push_back(_device);
//	m_linkedPhysicalDevices.push_back(_physDevice);
//	m_commandPools.push_back(vk::CommandPool());
//
//	UpdateBuffers(m_linkedDevices.size() - 1);
//}
//
//void DrawBuffer::GenerateCommandPool(int _deviceIndex)
//{
//	vk::CommandPoolCreateInfo poolInfo;
//	poolInfo.sType = vk::StructureType::eCommandPoolCreateInfo;
//	poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
//
//}
//
//void DrawBuffer::GenerateDeviceResources(int _deviceIndex)
//{
//	m_deviceResources.push_back(DeviceDrawResources());
//	DeviceDrawResources& resources = m_deviceResources.back();
//
//	vk::Device device = m_linkedDevices[_deviceIndex];
//
//	vk::DescriptorSetAllocateInfo allocInfo;
//	allocInfo.sType = vk::StructureType::eDescriptorSetAllocateInfo;
//	allocInfo.descriptorPool = m_descriptorPool;
//	allocInfo.descriptorSetCount = 1;
//	allocInfo.pSetLayouts = &m_material->GetDescriptorSetLayouts()[1];
//
//	resources.descriptorSets = device.allocateDescriptorSets(allocInfo);
//}

