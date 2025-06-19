#include "Renderer/DrawBuffer.h"
#include "Renderer/VulkanHelperFunctions.h"

DrawBuffer::DrawBuffer(Material* _material) : m_material(_material)
{
	m_vertexData = new Vertex[MaxVertexCount];
	m_indexData = new uint16_t[MaxIndexCount];
	m_modelData = new glm::mat4x4[MaxModelCount];

	m_buffersGenerated = false;
	m_dirty = false;

	m_vertexCount = 0;
	m_indexCount = 0;
	m_instanceCount = 0;
	
	//Create a descriptor pool that can keep as many textures as our array size and up to 10 uniform buffers (camera data)
	vk::DescriptorPoolSize* sizes = new vk::DescriptorPoolSize[1];
	vk::DescriptorPoolCreateInfo poolInfo;

	sizes[0].type = vk::DescriptorType::eCombinedImageSampler;
	sizes[0].descriptorCount = TextureArrayCount;

	poolInfo.sType = vk::StructureType::eDescriptorPoolCreateInfo;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = sizes;
	poolInfo.maxSets = 32;

	m_descriptorPool = VulkanEngine::LogicalDevice.createDescriptorPool(poolInfo);

	vk::DescriptorSetAllocateInfo allocInfo;
	allocInfo.sType = vk::StructureType::eDescriptorSetAllocateInfo;
	allocInfo.descriptorPool = m_descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &m_material->GetDescriptorSetLayouts()[1];

	vk::Device device = VulkanEngine::LogicalDevice;
	m_descriptorSets = device.allocateDescriptorSets(allocInfo);
}

int DrawBuffer::RemainingVertexPlaces()
{
	return (MaxVertexCount - m_vertexCount);
}

bool DrawBuffer::MeshCanFit(MeshData _mesh)
{
	return (_mesh.vertexCount + m_vertexCount <= MaxVertexCount) && (_mesh.triangleCount * 3 + m_indexCount <= MaxIndexCount);
}

bool DrawBuffer::TryAddMesh(MeshData* _meshInstance, glm::mat4x4 _modelMatrice, std::vector<TextureData*> _textures)
{
	int vertexCount = _meshInstance->vertexCount;

	if (!MeshCanFit(*_meshInstance))
		throw new std::exception("Not enough place in DrawBuffer");

	//Generate the texture index list
	for (auto it = _textures.begin(); it != _textures.end(); it++)
	{
		auto textureIt = std::find(m_textures.begin(), m_textures.end(), (*it));
		if (textureIt == m_textures.end())
		{
			//Need to add a new texture to the list
			m_textureIndexes.push_back(m_textures.size());
			m_textures.push_back(*it);

			m_texturesDirty = true;
		}
		else
		{
			//Texture alread exists
			m_textureIndexes.push_back(std::distance(m_textures.begin(), textureIt));
		}
	}

	//Add the mesh to the buffer
	auto it = std::find(m_meshes.begin(), m_meshes.end(), _meshInstance);
	int index = m_meshes.size();
	if (it == m_meshes.end())
	{
		//If it doesn't exist already we have to add the vertex and index datas
		Vertex* vertexBuffer = m_vertexData + m_vertexCount;
		uint16_t* indexBuffer = m_indexData + m_indexCount;

		int indexCount = 3 * _meshInstance->triangleCount;

		memcpy(vertexBuffer, _meshInstance->vertices, sizeof(Vertex) * vertexCount);

		for (int i = 0; i < indexCount; i++)
		{
			indexBuffer[i] = _meshInstance->indices[i] + m_vertexCount;
		}
		//memcpy(indexBuffer, _mesh->indices, 16 * 3 * _mesh->triangleCount);

		m_vertexCount += vertexCount;
		m_indexCount += _meshInstance->triangleCount * 3;

		m_meshes.push_back(_meshInstance);

		m_modelMatrices.push_back(std::vector<glm::mat4x4>());
		m_meshInstanceCount.push_back(0);
	}
	else
	{
		index = std::distance(m_meshes.begin(), it);
	}

	m_modelMatrices[index].push_back(_modelMatrice);
	m_meshInstanceCount[index]++;
	m_instanceCount++;

	GenerateModelMatriceBuffer();

	m_dirty = true;

	return true;
}

void DrawBuffer::GenerateBuffers()
{
	UpdateTextures();

	if (!m_dirty)
		return;

	if (m_buffersGenerated)
	{
		vk::Device device = VulkanEngine::LogicalDevice;

		device.freeMemory(m_vertexMemory);
		device.freeMemory(m_indexMemory);
		device.freeMemory(m_modelMatriceMemory);
	}

	BufferCreateInfo info =
	{
		.buffer = m_vertexBuffer,
		.memory = m_vertexMemory,
		.usage = vk::BufferUsageFlagBits::eVertexBuffer,
		.properties = vk::MemoryPropertyFlagBits::eDeviceLocal,
		.size = sizeof(Vertex) * m_vertexCount
	};

	vhf::CreateBufferWithStaging(info, m_vertexData);

	BufferCreateInfo indexInfo =
	{
		.buffer = m_indexBuffer,
		.memory = m_indexMemory,
		.usage = vk::BufferUsageFlagBits::eIndexBuffer,
		.size = (uint64_t) sizeof(uint16_t) * m_indexCount
	};

	vhf::CreateBufferWithStaging(indexInfo, m_indexData);
	
	int textureCountPerInstance = m_material->GetTextureCount();
	size_t instanceSize = 64 + 4 * textureCountPerInstance;
	size_t instanceBufferTotalSize = instanceSize * m_instanceCount;

	BufferCreateInfo modelBufferInfo =
	{
		.buffer = m_modelMatriceBuffer,
		.memory = m_modelMatriceMemory,
		.usage = vk::BufferUsageFlagBits::eVertexBuffer,
		.size = (uint64_t)instanceBufferTotalSize
	};

	char* instanceBuffer = new char[instanceBufferTotalSize];
	int* textureIndexes = m_textureIndexes.data();

	for (int i = 0; i < m_instanceCount; i++)
	{
		memcpy(instanceBuffer + (instanceSize * i), &m_modelData[i], 64);
		memcpy(instanceBuffer + (instanceSize * i) + 64, textureIndexes + (textureCountPerInstance * i), sizeof(int) * textureCountPerInstance);
	}

	vhf::CreateBufferWithStaging(modelBufferInfo, instanceBuffer);

	vk::Device device = VulkanEngine::LogicalDevice;

	int drawCount = m_meshes.size();
	uint64_t bufferSize = drawCount * sizeof(vk::DrawIndexedIndirectCommand);

	BufferCreateInfo drawCommandBufferInfo = BufferCreateInfo{
		.buffer = m_drawCommandBuffer,
		.memory = m_drawCommandMemory,
		.usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eIndirectBuffer,
		.properties = vk::MemoryPropertyFlagBits::eDeviceLocal,
		.size = bufferSize
	};

	int currIndex = 0;
	int currInstance = 0;

	m_drawCommands.resize(drawCount);

	for (int i = 0; i < drawCount; i++)
	{
		int instanceCount = m_meshInstanceCount[i];
		int indexCount = m_meshes[i]->triangleCount * 3;

		m_drawCommands[i].firstIndex = currIndex;
		m_drawCommands[i].firstInstance = currInstance;
		m_drawCommands[i].indexCount = indexCount;
		m_drawCommands[i].instanceCount = instanceCount;
		m_drawCommands[i].vertexOffset = 0;

		currInstance += instanceCount;
		currIndex += indexCount;
	}



	vhf::CreateBufferWithStaging(drawCommandBufferInfo, m_drawCommands.data());

	m_buffersGenerated = true;
	m_dirty = false;
}

void DrawBuffer::UpdateTextures()
{
	if (!m_texturesDirty)
		return;

	vk::Device device = VulkanEngine::LogicalDevice;
	size_t textureCount = m_textures.size();
	vk::WriteDescriptorSet* writeSets = new vk::WriteDescriptorSet[textureCount];
	vk::DescriptorImageInfo* imageInfos = new vk::DescriptorImageInfo[textureCount];

	for (int i = 0; i < textureCount; i++)
	{
		
		imageInfos[i].imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		imageInfos[i].imageView = m_textures[i]->m_imageView;
		imageInfos[i].sampler = m_textures[i]->m_sampler;

		writeSets[i].sType = vk::StructureType::eWriteDescriptorSet;
		writeSets[i].dstSet = m_descriptorSets[0];
		writeSets[i].dstBinding = 0;
		writeSets[i].dstArrayElement = i;
		writeSets[i].descriptorType = vk::DescriptorType::eCombinedImageSampler;
		writeSets[i].descriptorCount = 1;
		writeSets[i].pImageInfo = &imageInfos[i];
	}

	device.updateDescriptorSets(textureCount, writeSets, 0, nullptr);

	m_texturesDirty = false;
}

void DrawBuffer::RenderBuffer(vk::CommandBuffer _cmd, vk::DescriptorSet* _uboSet, int _currentPass)
{
	if (!m_buffersGenerated || m_dirty)
		return;

	vk::DeviceSize offsets[] = { 0 };

	_cmd.bindVertexBuffers(0, m_vertexBuffer, offsets);
	_cmd.bindVertexBuffers(1, m_modelMatriceBuffer, offsets);
	_cmd.bindIndexBuffer(m_indexBuffer, 0, vk::IndexType::eUint16);

	m_material->RecordCommandBuffer(_cmd, _currentPass, vk::PipelineBindPoint::eGraphics);

	_cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_material->GetLayouts()[0], 0, 1, _uboSet, 0, nullptr);
	_cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_material->GetLayouts()[0], 1, 1, m_descriptorSets.data(), 0, nullptr);

	int currIndex = 0;
	int currInstance = 0;

	for (int i = 0; i < m_meshes.size(); i++)
	{
		int instanceCount = m_meshInstanceCount[i];
		int indexCount = m_meshes[i]->triangleCount * 3;

		_cmd.drawIndexed(indexCount, instanceCount, currIndex, 0, currInstance);

		currInstance += instanceCount;
		currIndex += indexCount;
	}
}

void DrawBuffer::RenderBufferIndirect(vk::CommandBuffer _cmd, vk::DescriptorSet* _uboSet, int _currentPass)
{
	if (!m_buffersGenerated || m_dirty)
		return;

	vk::DeviceSize offsets[] = { 0 };

	_cmd.bindVertexBuffers(0, m_vertexBuffer, offsets);
	_cmd.bindVertexBuffers(1, m_modelMatriceBuffer, offsets);
	_cmd.bindIndexBuffer(m_indexBuffer, 0, vk::IndexType::eUint16);

	m_material->RecordCommandBuffer(_cmd, _currentPass, vk::PipelineBindPoint::eGraphics);

	_cmd.drawIndexedIndirect(m_drawCommandBuffer, 0, m_meshes.size(), sizeof(vk::DrawIndexedIndirectCommand));
}

void DrawBuffer::GenerateModelMatriceBuffer()
{
	int index = 0;

	for (auto meshIt = m_modelMatrices.begin(); meshIt != m_modelMatrices.end(); meshIt++)
	{
		for (auto it = (*meshIt).begin(); it != (*meshIt).end(); it++)
		{
			m_modelData[index] = (*it);

			index++;
		}
	}
}

