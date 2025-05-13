#pragma once

#include "vulkan/vulkan.hpp"
#include "common.h"


class MaterialInstance;
class Renderer;
class Material;

class Material
{
public:
	Material(Renderer* _renderer, vk::Device _device, int _textureCount);
	//Material(VulkanRenderer* _renderer, vk::Pipeline& _defaultDepthPipeline);
	virtual std::vector<vk::PipelineLayout> GetLayouts() { return m_layouts; };
	virtual void RecordCommandBuffer(vk::CommandBuffer _buffer, int _renderPass, vk::PipelineBindPoint _bindPoint);


	virtual MaterialInstance* CreateInstance(vk::Device _device, vk::DescriptorSetLayout* _pSetLayouts, vk::DescriptorPool _pool, int _layoutCount);
private:
	int m_textureCount;

	std::vector<vk::Pipeline> m_pipelines;
	std::vector<vk::PipelineLayout> m_layouts;

	virtual void CreatePipelines(Renderer* _renderer, vk::Device _device);
};

class MaterialInstance
{
private:
	Material& m_material;

public:
	MaterialInstance(Material& _material) : m_material(_material) {};
	void AllocateSets(vk::Device _device, vk::DescriptorSetLayout* _pSetLayouts, vk::DescriptorPool _descriptorPool, int _count);
	void UpdateSets(vk::Device _device, std::vector<vk::ImageView> _views, std::vector<vk::Sampler> _samplers);

	void BindSets(vk::CommandBuffer _buffer, vk::PipelineLayout _layout);
	void CleanUp(vk::Device _device, vk::DescriptorPool _pool);
	void RecordCommandBuffer(vk::CommandBuffer& _buffer, int _renderPass, vk::PipelineBindPoint _bindPoint);

	vk::DescriptorSet* m_descriptorSets;
	int m_descriptorSetCount;
};