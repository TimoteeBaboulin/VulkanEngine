#pragma once

#include <vulkan/vulkan.hpp>
#include "common.h"

constexpr int TextureArrayCount = 32;

class RenderTarget;

struct ShaderCode;

//TODO: Add the base material pointer to be able to read layout data
/// <summary>
/// Represents an instance of a material, with a specific device in mind
/// </summary>
class MaterialInstance
{
public:
	MaterialInstance(RenderTarget& _target, ShaderCode* _shaderCodes);
	std::vector<vk::PipelineLayout> GetLayouts() { return m_pipelineLayouts; };
	void RecordCommandBuffer(vk::CommandBuffer _buffer, int _renderPass, vk::PipelineBindPoint _bindPoint);

	//GETTERS
	int GetTextureCount() const { return m_textureCount; }
	const vk::DescriptorSetLayout* GetDescriptorSetLayouts() const { return m_setLayouts.data(); }
	vk::DescriptorPool GetDescriptorPool() const { return m_descriptorPool; }
private:
	int m_textureCount;

	RenderTarget& m_target;
	ShaderCode* m_shaderCodes;
	DeviceData m_deviceData;

	std::vector<vk::Pipeline> m_pipelines;
	std::vector<vk::PipelineLayout> m_pipelineLayouts;
	std::vector<vk::DescriptorSetLayout> m_setLayouts;
	vk::DescriptorPool m_descriptorPool;
	
	void CreatePipelineLayouts();
	void CreatePipelines();
	void CreateDescriptorPool();
};