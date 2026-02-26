#pragma once

#include "../../../common.h"
#include "../CustomVulkanStructs.h"

#include "Material.h"

constexpr int TextureArrayCount = 32;

class RenderTarget;

namespace Moonlit
{
	/// <summary>
	/// Represents an instance of a material, with a specific device in mind
	/// </summary>
	class MaterialInstance
	{
	public:
		MaterialInstance(vk::Device _device, Material* _material, vk::Format _colorOutputFormat);
		~MaterialInstance();
		void BindPipeline(vk::CommandBuffer _buffer, std::string _renderPass);

		//GETTERS
		int GetTextureCount() const { return m_textureCount; }
		const vk::DescriptorSetLayout* GetDescriptorSetLayouts() const { return m_setLayouts.data(); }
		vk::DescriptorPool GetDescriptorPool() const { return m_descriptorPool; }
		std::vector<vk::PipelineLayout> GetLayouts() const { return m_pipelineLayouts; };
	private:
		// Data
		int m_textureCount;
		ShaderData m_shaderData;
		Material* m_baseMaterial;
		vk::Device m_device;
		vk::Format m_colorOutputFormat;

		// Vulkan Items
		std::map<std::string, vk::Pipeline> m_pipelines;
		std::vector<vk::PipelineLayout> m_pipelineLayouts;
		std::vector<vk::DescriptorSetLayout> m_setLayouts;
		vk::DescriptorPool m_descriptorPool;

		void CreateUboDescriptorSetLayout();
		void CreateTextureDescriptorSetLayout();

		void CreatePipelineLayouts();
		void CreatePipelines();
		void CreateDescriptorPool();
	};
}