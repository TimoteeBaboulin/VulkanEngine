#include "MoonlitVulkan/VulkanPipeline.h"
#include "MoonlitVulkan/VulkanEngine.h"
#include "MoonlitVulkan/VulkanHelperFunctions.h"

#include "MoonlitVulkan/VulkanData.h"

VulkanPipeline::VulkanPipeline(VulkanPipelineInfo _info) : m_info(_info)
{
}

void VulkanPipeline::Init(vk::Extent2D _extent)
{
	CreateSwapChain(_extent);
	/*CreateImageViews()*/;

	CreateDepthImage();
	/*CreateRenderPasses();
	CreateDescriptorSetLayout();
	CreateRenderPipeline();*/

	CreateFrameBuffers();
}

void VulkanPipeline::Cleanup()
{
	vk::Device& device = VulkanEngine::LogicalDevice;

	for (auto frameBuffer : m_frameBuffers)
	{
		device.destroyFramebuffer(frameBuffer);
	}

	device.destroyPipeline(m_pipeline);
	device.destroyPipeline(m_depthPipeline);
	device.destroyPipelineLayout(m_pipelineLayout);

	device.destroyDescriptorSetLayout(m_uboDescriptorLayout);
	device.destroyDescriptorSetLayout(m_shaderDescriptorLayout);

	device.destroyRenderPass(m_renderPass);
	device.unmapMemory(m_depthMemory);

	for (int i = 0; i < m_frameCount; i++)
	{
		device.destroyImageView(m_imageViews[i]);
		device.destroyImageView(m_depthImageViews[i]);
		device.destroyImage(m_images[i]);
		device.destroyImage(m_depthImages[i]);
	}

	device.destroySwapchainKHR(m_swapChain);
}

RenderInfo VulkanPipeline::GetRenderInfo()
{
	RenderInfo info = {
		.renderPass = m_renderPass,
		.pipeline = m_pipeline,
		.depthPipeline = m_depthPipeline,
		.pipelineLayout = m_pipelineLayout
	};

	return info;
}



void VulkanPipeline::CreateSwapChain(vk::Extent2D _extent)
{
	
}



void VulkanPipeline::CreateDepthImage()
{
	vk::Format format = vk::Format::eD32Sfloat;
	uint32_t queueFamilyIndices[] = {VulkanEngine::FamilyIndices.graphicsFamily.value(), VulkanEngine::FamilyIndices.khrPresentFamily.value()};

	m_depthImages.resize(m_frameCount);
	m_depthImageViews.resize(m_frameCount);

	//vk::ImageCreateInfo imageInfo;
	//imageInfo.sType = vk::StructureType::eImageCreateInfo;
	//imageInfo.extent = vk::Extent3D(m_extent);
	//imageInfo.format = vk::Format::eD32Sfloat;
	//imageInfo.imageType = vk::ImageType::e2D;
	//imageInfo.queueFamilyIndexCount = 2;
	//imageInfo.pQueueFamilyIndices = queueFamilyIndices;
	//imageInfo.mipLevels = 1;
	//imageInfo.arrayLayers = 1;
	//imageInfo.tiling = vk::ImageTiling::eOptimal;
	//imageInfo.samples = vk::SampleCountFlagBits::e1;

	for (int i = 0; i < m_frameCount; i++)
	{
		vhf::CreateImage(m_extent, format, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eDepthStencilAttachment, vk::MemoryPropertyFlagBits::eDeviceLocal, m_depthImages[i], m_depthMemory, vk::ImageLayout::eUndefined);
		m_depthImageViews[i] = vhf::CreateImageView(m_depthImages[i], format, vk::ImageAspectFlagBits::eDepth);
	}
}


void VulkanPipeline::CreateFrameBuffers()
{
	m_frameBuffers.reserve(m_imageViews.size());

	for (size_t i = 0; i < m_imageViews.size(); i++)
	{
		//std::vector<ImageView>
		std::vector<vk::ImageView> attachments = { m_depthImageViews[i], m_imageViews[i]};

		vk::FramebufferCreateInfo createInfo{};
		createInfo.sType = vk::StructureType::eFramebufferCreateInfo;
		createInfo.renderPass = m_renderPass;
		createInfo.height = m_extent.height;
		createInfo.width = m_extent.width;
		createInfo.attachmentCount = 2;
		createInfo.pAttachments = attachments.data();
		createInfo.layers = 1;
		m_frameBuffers.push_back(VulkanEngine::LogicalDevice.createFramebuffer(createInfo));
	}
}

vk::Extent2D VulkanPipeline::GetExtent(vk::SurfaceCapabilitiesKHR _capabilities)
{
	if (_capabilities.currentExtent != std::numeric_limits<uint32_t>::max())
		return _capabilities.currentExtent;

	int width = 800;
	int height = 800;
	//glfwGetFramebufferSize(m_window, &width, &height);

	uint32_t uwidth = (uint32_t)width;
	uint32_t uheight = (uint32_t)height;

	uwidth = std::clamp(uwidth, _capabilities.minImageExtent.width, _capabilities.maxImageExtent.width);
	uheight = std::clamp(uheight, _capabilities.minImageExtent.height, _capabilities.maxImageExtent.height);

	return vk::Extent2D(uwidth, uheight);
}