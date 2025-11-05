#pragma once

#include "common.h"
#include "Engine/Renderer/CustomVulkanStructs.h"

constexpr int TextureArrayCount = 32;

class RenderTarget;
class Material;

struct ShaderCode;

/// <summary>
/// Represents an instance of a material, with a specific device in mind
/// </summary>
class MaterialInstance
{
public:
	MaterialInstance(RenderTarget& _target, ShaderCode* _shaderCodes, Material* _material);
	void RecordCommandBuffer(vk::CommandBuffer _buffer, int _renderPass, vk::PipelineBindPoint _bindPoint);

	//GETTERS
	int GetTextureCount() const { return m_textureCount; }
	const vk::DescriptorSetLayout* GetDescriptorSetLayouts() const { return m_setLayouts.data(); }
	vk::DescriptorPool GetDescriptorPool() const { return m_descriptorPool; }
	std::vector<vk::PipelineLayout> GetLayouts() const { return m_pipelineLayouts; };
private:
	// Data
	int m_textureCount;
	RenderTarget& m_target;
	ShaderCode* m_shaderCodes;
	Material* m_baseMaterial;
	DeviceData m_deviceData;

	// Vulkan Items
	std::vector<vk::Pipeline> m_pipelines;
	std::vector<vk::PipelineLayout> m_pipelineLayouts;
	std::vector<vk::DescriptorSetLayout> m_setLayouts;
	vk::DescriptorPool m_descriptorPool;
	
	void CreatePipelineLayouts();
	void CreatePipelines();
	void CreateDescriptorPool();
};