#pragma once

#include "vulkan/vulkan.hpp"
#include "common.h"

struct VulkanPipelineInfo
{
	SwapChainSupportDetails details;

	vk::SurfaceKHR& surface;
};

class VulkanPipeline
{
public:
	VulkanPipeline(VulkanPipelineInfo _info);
	void Init(vk::Extent2D _extent);
	void Init(vk::Extent2D _extent, Mesh& _mesh);

	void Cleanup();

	vk::Pipeline GetPipeline() const { return m_pipeline; }
	vk::RenderPass GetRenderPass() const { return m_renderPass; }
	vk::SwapchainKHR GetSwapChain() const { return m_swapChain; }
	vk::DescriptorSetLayout GetDescriptorSetLayout() const { return m_descriptorLayout; }
	std::vector<vk::Framebuffer>* GetFrameBuffers() { return &m_frameBuffers; }
	std::vector<vk::ImageView> GetImageViews() { return m_imageViews; }

	vk::ImageView GetTextureView(int index = 0) { return m_textureImageViews[index]; }
	vk::Sampler GetSampler() { return m_textureSampler; }

	RenderInfo GetRenderInfo();

	void Load(Mesh& _mesh);
	void AddTexture(Image _texture);
private:
	VulkanPipelineInfo m_info;

	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;

	vk::SwapchainKHR m_swapChain;
	std::vector<vk::Image> m_images;
	std::vector<vk::ImageView> m_imageViews;

	std::vector<vk::Image> m_depthImages;
	std::vector<vk::ImageView> m_depthImageViews;
	vk::DeviceMemory m_depthMemory;

	std::vector<vk::Image> m_textureImages;
	std::vector<vk::ImageView> m_textureImageViews;
	vk::Sampler m_textureSampler;

	std::vector<vk::Framebuffer> m_frameBuffers;

	vk::Extent2D m_extent;
	vk::SurfaceFormatKHR m_format;
	vk::RenderPass m_renderPass;
	vk::Pipeline m_pipeline;
	vk::Pipeline m_depthPipeline;
	vk::PipelineLayout m_pipelineLayout;
	
	vk::Buffer m_vertexBuffer = nullptr;
	vk::DeviceMemory m_vertexMemory;
	vk::Buffer m_indexBuffer = nullptr;
	vk::DeviceMemory m_indexMemory;

	bool vertexBufferCreated = false;
	bool indexBufferCreated = false;

	uint32_t m_vertexCount;
	uint32_t m_triangleCount;
	uint32_t m_frameCount;

	vk::DescriptorSetLayout m_descriptorLayout;

	//void CreateMainCommandPool();
	void CreateDescriptorSetLayout();
	void CreateSwapChain(vk::Extent2D _extent);
	void CreateImageViews();
	void CreateDepthImage();
	void CreateRenderPasses();
	void CreatePipelineLayout();
	void CreateRenderPipeline();

	void CreateFrameBuffers();
	void CreateTextureImages();
	void CreateTextureSampler();

	void CreateVertexBuffer();
	void CreateIndexBuffer();

	void AddMesh(Mesh _mesh);

	vk::ShaderModule WrapShader(std::vector<char> _shaderBytes);
	vk::SurfaceFormatKHR GetFormat(std::vector<vk::SurfaceFormatKHR>& _format);
	vk::PresentModeKHR GetPresentMode(std::vector<vk::PresentModeKHR>& _modes);
	vk::Extent2D GetExtent(vk::SurfaceCapabilitiesKHR _capabilities);
};