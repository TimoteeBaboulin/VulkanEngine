#pragma once

#include "vulkan/vulkan.hpp"
#include "common.h"

constexpr int TextureArrayCount = 32;

class Renderer;
class Material;

class Material
{
public:
	Material(vk::Device _device, int _textureCount,
		vk::RenderPass _renderPass, vk::DescriptorSetLayout _uboLayout);
	virtual std::vector<vk::PipelineLayout> GetLayouts() { return m_pipelineLayouts; };
	virtual void RecordCommandBuffer(vk::CommandBuffer _buffer, int _renderPass, vk::PipelineBindPoint _bindPoint);

	int GetTextureCount() const { return m_textureCount; }
	const vk::DescriptorSetLayout* GetDescriptorSetLayouts() const { return m_setLayouts.data(); }
private:
	int m_textureCount;

	std::vector<vk::Pipeline> m_pipelines;
	std::vector<vk::PipelineLayout> m_pipelineLayouts;
	std::vector<vk::DescriptorSetLayout> m_setLayouts;

	virtual void CreatePipelines(vk::Device _device, vk::RenderPass _renderPass);
	virtual void CreatePipelineLayouts(vk::Device& _device, vk::DescriptorSetLayout _uboLayout);
};