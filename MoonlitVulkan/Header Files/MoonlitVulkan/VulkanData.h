#pragma once

#include "common.h"
#include "vulkan/vulkan.hpp"

class Material;

class Mesh
{
public:
	void Load(vk::Device device, MeshData _data, vk::DescriptorSetLayout _layout, vk::DescriptorPool _pool);
	void BindSets(vk::CommandBuffer _buffer, vk::PipelineLayout _layout);

	MeshCountType m_vertexCount;
	MeshCountType m_triangleCount;

	uint16_t m_textureCount;

	vk::Buffer m_vertexBuffer;
	vk::Buffer m_indexBuffer;

	vk::DeviceMemory m_vertexMemory;
	vk::DeviceMemory m_indexMemory;

	std::vector<vk::Image> m_textureImages;
	std::vector<vk::ImageView> m_textureImageViews;
	std::vector<vk::Sampler> m_textureSamplers;

	std::vector<Material> m_materials;
};

class Material
{
public:
	void AllocateSets(vk::Device _device, vk::DescriptorSetLayout _descriptorLayout, vk::DescriptorPool _descriptorPool, int _count);
	void UpdateSets(vk::Device _device, std::vector<vk::ImageView> _views, std::vector<vk::Sampler> _samplers);
	
	void BindSets(vk::CommandBuffer _buffer, vk::PipelineLayout _layout);

	std::vector<vk::DescriptorSet> m_descriptorSets;
	int m_descriptorSetCount;
};