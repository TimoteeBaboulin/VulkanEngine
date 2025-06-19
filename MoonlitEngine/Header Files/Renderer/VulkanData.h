#pragma once

#include "vulkan/vulkan.hpp"
#include "common.h"

constexpr int TextureArrayCount = 32;

class MaterialInstance;
class Renderer;
class Material;

class Material
{
public:
	Material(Renderer* _renderer, vk::Device _device, int _textureCount);
	//Material(VulkanRenderer* _renderer, vk::Pipeline& _defaultDepthPipeline);
	virtual std::vector<vk::PipelineLayout> GetLayouts() { return m_pipelineLayouts; };
	virtual void RecordCommandBuffer(vk::CommandBuffer _buffer, int _renderPass, vk::PipelineBindPoint _bindPoint);

	virtual MaterialInstance* CreateInstance(vk::Device _device, vk::DescriptorPool _pool, vk::ImageView* _views, vk::Sampler* _samplers);

	int GetTextureCount() const { return m_textureCount; }
	const vk::DescriptorSetLayout* GetDescriptorSetLayouts() const { return m_setLayouts.data(); }
private:
	int m_textureCount;

	std::vector<vk::Pipeline> m_pipelines;
	std::vector<vk::PipelineLayout> m_pipelineLayouts;
	std::vector<vk::DescriptorSetLayout> m_setLayouts;

	virtual void CreatePipelines(Renderer* _renderer, vk::Device _device);
	virtual void CreatePipelineLayouts(Renderer& _renderer, vk::Device& _device);
};

class MaterialInstance
{
private:
	Material& m_material;

public:
	MaterialInstance(Material& _material) : m_material(_material), m_descriptorSets(nullptr), m_descriptorSetCount(){};
	void AllocateSets(vk::Device _device, vk::DescriptorSetLayout* _pSetLayouts, vk::DescriptorPool _descriptorPool, int _count);
	void UpdateSets(vk::Device _device, vk::ImageView* _views, vk::Sampler* _samplers);

	void BindSets(vk::CommandBuffer _buffer, vk::PipelineLayout _layout);
	void CleanUp(vk::Device _device, vk::DescriptorPool _pool);
	void RecordCommandBuffer(vk::CommandBuffer& _buffer, int _renderPass, vk::PipelineBindPoint _bindPoint, vk::DescriptorSet* _uboSet);

	Material& GetMaterial() const { return m_material; }

	vk::DescriptorSet* m_descriptorSets;
	int m_descriptorSetCount;
};