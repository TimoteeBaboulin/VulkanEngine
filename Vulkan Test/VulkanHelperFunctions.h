
#ifndef VULKAN_HELPER_FUNCTIONS_H
#define VULKAN_HELPER_FUNCTIONS_H

#include <cstdint>
#include "vulkan/vulkan.hpp"
#include "VulkanEngine.h"

class VulkanHelperFunctions
{
public:
	static uint32_t FindMemoryTypeIndex(uint32_t _typeFilter, vk::MemoryPropertyFlags _properties, vk::PhysicalDevice _phys)
	{
		vk::PhysicalDeviceMemoryProperties properties;
		properties = _phys.getMemoryProperties();

		for (size_t i = 0; i < properties.memoryTypeCount; i++)
		{
			if (_typeFilter & (1 << i) && (properties.memoryTypes[i].propertyFlags & _properties) == _properties)
				return i;
		}

		throw new std::runtime_error("couldn't allocate memory");
	}

	static vk::CommandBuffer BeginSingleUseCommand(vk::CommandPool& _pool, vk::Device& _device)
	{
		vk::CommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = vk::StructureType::eCommandBufferAllocateInfo;
		allocInfo.level = vk::CommandBufferLevel::ePrimary;
		allocInfo.commandPool = _pool;
		allocInfo.commandBufferCount = 1;

		vk::CommandBuffer commandBuffer;
		_device.allocateCommandBuffers(&allocInfo, &commandBuffer);

		vk::CommandBufferBeginInfo beginInfo{};
		beginInfo.sType = vk::StructureType::eCommandBufferBeginInfo;
		beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

		commandBuffer.begin(beginInfo);

		return commandBuffer;
	}

	static void EndSingleUseCommandBuffer(vk::CommandBuffer& _buffer, vk::Queue& _queue, vk::Device& _device, vk::CommandPool& _commandPool)
	{
		_buffer.end();

		vk::SubmitInfo submitInfo{};
		submitInfo.sType = vk::StructureType::eSubmitInfo;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &_buffer;

		_queue.submit(1, &submitInfo, nullptr);
		_queue.waitIdle();

		_device.freeCommandBuffers(_commandPool, 1, &_buffer);
	}


	static void CreateImage(vk::Extent2D _extent, vk::Format _format, vk::ImageTiling _tiling, vk::ImageUsageFlags _features, vk::MemoryPropertyFlags _properties, vk::Image& _image, vk::DeviceMemory& _memory, vk::ImageLayout _defaultLayout)
	{
		vk::ImageCreateInfo info;
		info.sType = vk::StructureType::eImageCreateInfo;
		info.imageType = vk::ImageType::e2D;
		info.extent = vk::Extent3D{ _extent.width, _extent.height, 1 };
		info.usage = _features;
		info.format = _format;
		info.tiling = _tiling;
		info.mipLevels = 1;
		info.arrayLayers = 1;
		info.initialLayout = _defaultLayout;
		info.samples = vk::SampleCountFlagBits::e1;
		info.sharingMode = vk::SharingMode::eExclusive;

		_image = VulkanEngine::LogicalDevice.createImage(info);

		vk::MemoryRequirements memReqs = VulkanEngine::LogicalDevice.getImageMemoryRequirements(_image);

		vk::MemoryAllocateInfo allocInfo;
		allocInfo.sType = vk::StructureType::eMemoryAllocateInfo;
		allocInfo.allocationSize = memReqs.size;
		allocInfo.memoryTypeIndex = VulkanHelperFunctions::FindMemoryTypeIndex(memReqs.memoryTypeBits, _properties, VulkanEngine::PhysicalDevice);

		_memory = VulkanEngine::LogicalDevice.allocateMemory(allocInfo);
		VulkanEngine::LogicalDevice.bindImageMemory(_image, _memory, 0);
	}

	static vk::ImageView CreateImageView(vk::Image& _image, vk::Format _format, vk::ImageAspectFlags _aspect)
	{
		vk::ImageViewCreateInfo info;
		info.sType = vk::StructureType::eImageViewCreateInfo;
		info.format = _format;
		info.image = _image;
		info.subresourceRange.aspectMask = _aspect;
		info.subresourceRange.baseArrayLayer = 0;
		info.subresourceRange.baseMipLevel = 0;
		info.subresourceRange.levelCount = 1;
		info.subresourceRange.layerCount = 1;
		info.viewType = vk::ImageViewType::e2D;

		return VulkanEngine::LogicalDevice.createImageView(info);
	}

	static void TransitionImageLayout(vk::Image& _image, vk::Format _format, vk::ImageAspectFlags _aspect, vk::ImageLayout _src, vk::ImageLayout _dst, TransitionInfo _transition)
	{
		vk::CommandBuffer buffer = BeginSingleUseCommand(VulkanEngine::MainCommandPool, VulkanEngine::LogicalDevice);
		
		vk::ImageMemoryBarrier barrier;
		barrier.sType = vk::StructureType::eImageMemoryBarrier;
		barrier.oldLayout = _src;
		barrier.newLayout = _dst;
		barrier.srcQueueFamilyIndex = vk::QueueFamilyIgnored;
		barrier.dstQueueFamilyIndex = vk::QueueFamilyIgnored;
		barrier.image = _image;
		barrier.subresourceRange.aspectMask = _aspect;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		barrier.srcAccessMask = _transition.srcAccessFlags;
		barrier.dstAccessMask = _transition.dstAccessFlags;

		buffer.pipelineBarrier(_transition.srcStage, _transition.dstStage, vk::DependencyFlagBits::eByRegion, std::vector<vk::MemoryBarrier>(), std::vector<vk::BufferMemoryBarrier>(), std::vector<vk::ImageMemoryBarrier>{barrier});

		EndSingleUseCommandBuffer(buffer, VulkanEngine::GraphicsQueue, VulkanEngine::LogicalDevice, VulkanEngine::MainCommandPool);
	}

	static void CopyBufferToBuffer(vk::Buffer& _src, vk::Buffer& _dst, uint64_t _size, vk::Queue _queue)
	{
		vk::CommandBufferAllocateInfo alloc;
		alloc.sType = vk::StructureType::eCommandBufferAllocateInfo;
		alloc.commandBufferCount = 1;
		alloc.level = vk::CommandBufferLevel::ePrimary;
		alloc.commandPool = VulkanEngine::MainCommandPool;

		vk::CommandBuffer command = VulkanEngine::LogicalDevice.allocateCommandBuffers(alloc)[0];

		vk::CommandBufferBeginInfo begin;
		begin.sType = vk::StructureType::eCommandBufferBeginInfo;
		begin.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

		command.begin(begin);

		vk::BufferCopy buffCopy;
		buffCopy.srcOffset = 0;
		buffCopy.dstOffset = 0;
		buffCopy.size = _size;

		command.copyBuffer(_src, _dst, buffCopy);

		command.end();

		vk::SubmitInfo submit;
		submit.sType = vk::StructureType::eSubmitInfo;
		submit.commandBufferCount = 1;
		submit.pCommandBuffers = &command;

		_queue.submit(submit);
		_queue.waitIdle();

		VulkanEngine::LogicalDevice.freeCommandBuffers(VulkanEngine::MainCommandPool, command);
	}

	static void CreateBuffer(VertexBufferInfo _info)
	{
		vk::BufferCreateInfo bufferInfo;
		bufferInfo.sType = vk::StructureType::eBufferCreateInfo;
		bufferInfo.size = _info.size;
		bufferInfo.usage = _info.usage;
		bufferInfo.sharingMode = vk::SharingMode::eExclusive;

		_info.buffer = VulkanEngine::LogicalDevice.createBuffer(bufferInfo);

		vk::MemoryRequirements memoryReqs;
		memoryReqs = VulkanEngine::LogicalDevice.getBufferMemoryRequirements(_info.buffer);

		vk::MemoryAllocateInfo memoryInfo;
		memoryInfo.sType = vk::StructureType::eMemoryAllocateInfo;
		memoryInfo.allocationSize = memoryReqs.size;
		memoryInfo.memoryTypeIndex = VulkanHelperFunctions::FindMemoryTypeIndex(memoryReqs.memoryTypeBits, _info.properties, VulkanEngine::PhysicalDevice);
		_info.memory = VulkanEngine::LogicalDevice.allocateMemory(memoryInfo);
		VulkanEngine::LogicalDevice.bindBufferMemory(_info.buffer, _info.memory, 0);
	}
};

typedef VulkanHelperFunctions vhf;

#endif