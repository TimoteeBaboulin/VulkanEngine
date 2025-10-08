#pragma once

#include <vulkan/vulkan.hpp>

constexpr int TextureArrayCount = 32;

struct ShaderCode;

/// <summary>
/// Represents an instance of a material, with a specific device in mind
/// </summary>
class MaterialInstance
{
public:
	MaterialInstance(RenderTarget& _target, ShaderCode* _shaderCodes);
	std::vector<vk::PipelineLayout> GetLayouts() { return m_pipelineLayouts; };
	void RecordCommandBuffer(vk::CommandBuffer _buffer, int _renderPass, vk::PipelineBindPoint _bindPoint);

	int GetTextureCount() const { return m_textureCount; }
	const vk::DescriptorSetLayout* GetDescriptorSetLayouts() const { return m_setLayouts.data(); }
private:
	int m_textureCount;

	RenderTarget& m_target;
	ShaderCode* m_shaderCodes;
	vk::Device m_device;

	std::vector<vk::Pipeline> m_pipelines;
	std::vector<vk::PipelineLayout> m_pipelineLayouts;
	std::vector<vk::DescriptorSetLayout> m_setLayouts;

	
	void CreatePipelineLayouts();
	void CreatePipelines();
};