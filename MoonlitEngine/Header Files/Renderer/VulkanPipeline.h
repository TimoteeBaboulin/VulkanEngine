#pragma once

#include "vulkan/vulkan.hpp"
#include "common.h"



class Mesh;

struct VulkanPipelineInfo
{
	SwapChainSupportDetails details;

	vk::SurfaceKHR& surface;
};

class VulkanPipeline
{
public:
	VulkanPipeline();
	void Init(vk::Extent2D _extent);

	void Cleanup();

	vk::Pipeline GetPipeline() const { return m_pipeline; }
	vk::RenderPass GetRenderPass() const { return m_renderPass; }
	vk::SwapchainKHR GetSwapChain() const { return m_swapChain; }
	vk::DescriptorSetLayout GetDescriptorSetLayout() const { return m_uboDescriptorLayout; }
	vk::DescriptorSetLayout GetShaderDescriptorSetLayout() const { return m_shaderDescriptorLayout; }
	std::vector<vk::Framebuffer>* GetFrameBuffers() { return &m_frameBuffers; }
	std::vector<vk::ImageView> GetImageViews() { return m_imageViews; }

	//RenderInfo GetRenderInfo();

	//void Load(MeshData& _mesh);
private:
	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;

	vk::SwapchainKHR m_swapChain;
	std::vector<vk::Image> m_images;
	std::vector<vk::ImageView> m_imageViews;

	std::vector<vk::Image> m_depthImages;
	std::vector<vk::ImageView> m_depthImageViews;
	vk::DeviceMemory m_depthMemory;

	std::vector<vk::Framebuffer> m_frameBuffers;

	vk::Extent2D m_extent;
	vk::SurfaceFormatKHR m_format;
	vk::RenderPass m_renderPass;
	vk::Pipeline m_pipeline;
	vk::Pipeline m_depthPipeline;
	vk::PipelineLayout m_pipelineLayout;

	uint32_t m_frameCount;

	vk::DescriptorSetLayout m_shaderDescriptorLayout;
	vk::DescriptorSetLayout m_uboDescriptorLayout;

	void CreateDepthImage();
	void CreateFrameBuffers();

	vk::ShaderModule WrapShader(std::vector<char> _shaderBytes);
	vk::SurfaceFormatKHR GetFormat(std::vector<vk::SurfaceFormatKHR>& _format);
	vk::PresentModeKHR GetPresentMode(std::vector<vk::PresentModeKHR>& _modes);
	//vk::Extent2D GetExtent(vk::SurfaceCapabilitiesKHR _capabilities);
};