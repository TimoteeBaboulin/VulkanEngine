
#ifndef VULKAN_HELPER_FUNCTIONS_H
#define VULKAN_HELPER_FUNCTIONS_H

#include <cstdint>
#include "common.h"
#include "Debug/Logger.h"

#define WIN32_LEAN_AND_MEAN

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
		vk::Result result = _device.allocateCommandBuffers(&allocInfo, &commandBuffer);

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

		vk::Result result = _queue.submit(1, &submitInfo, nullptr);
		if (result != vk::Result::eSuccess)
		{
			LOG_ERROR("Failed to submit command buffer!");
			throw std::runtime_error("failed to submit command buffer!");
		}

		_queue.waitIdle();

		_device.freeCommandBuffers(_commandPool, 1, &_buffer);
	}

	static void CreateImage(vk::Device _device, vk::PhysicalDevice _physicalDevice, vk::Extent2D _extent, vk::Format _format, vk::ImageTiling _tiling, vk::ImageUsageFlags _features, vk::MemoryPropertyFlags _properties, vk::Image& _image, vk::DeviceMemory& _memory, vk::ImageLayout _defaultLayout)
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

		_image = _device.createImage(info);

		vk::MemoryRequirements memReqs = _device.getImageMemoryRequirements(_image);

		vk::MemoryAllocateInfo allocInfo;
		allocInfo.sType = vk::StructureType::eMemoryAllocateInfo;
		allocInfo.allocationSize = memReqs.size;
		allocInfo.memoryTypeIndex = VulkanHelperFunctions::FindMemoryTypeIndex(memReqs.memoryTypeBits, _properties, _physicalDevice);

		_memory = _device.allocateMemory(allocInfo);
		_device.bindImageMemory(_image, _memory, 0);
	}

	static vk::ImageView CreateImageView(vk::Device _device, vk::Image& _image, vk::Format _format, vk::ImageAspectFlags _aspect)
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

		return _device.createImageView(info);
	}

	static void TransitionImageLayout(vk::Device _device, vk::CommandPool _commandPool, vk::Queue _graphicsQueue, vk::Image& _image, vk::Format _format, vk::ImageAspectFlags _aspect, vk::ImageLayout _src, vk::ImageLayout _dst, TransitionInfo _transition)
	{
		vk::CommandBuffer buffer = BeginSingleUseCommand(_commandPool, _device);
		
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

		std::vector<vk::ImageMemoryBarrier> imageBarriers = { barrier };
		std::vector<vk::MemoryBarrier> memBarriers;
		std::vector<vk::BufferMemoryBarrier> bufferBarriers;

		buffer.pipelineBarrier(_transition.srcStage, _transition.dstStage, vk::DependencyFlagBits::eByRegion, memBarriers, bufferBarriers, imageBarriers);

		EndSingleUseCommandBuffer(buffer, _graphicsQueue, _device, _commandPool);
	}

	static void CopyBufferToBuffer(vk::Device _device, vk::CommandPool _commandPool, vk::Buffer& _src, vk::Buffer& _dst, uint64_t _size, vk::Queue _queue)
	{
		vk::CommandBufferAllocateInfo alloc;
		alloc.sType = vk::StructureType::eCommandBufferAllocateInfo;
		alloc.commandBufferCount = 1;
		alloc.level = vk::CommandBufferLevel::ePrimary;
		alloc.commandPool = _commandPool;

		//vk::CommandBuffer command = VulkanEngine::LogicalDevice.allocateCommandBuffers(alloc)[0];
		vk::CommandBuffer command = BeginSingleUseCommand(_commandPool, _device);

		/*vk::CommandBufferBeginInfo begin;
		begin.sType = vk::StructureType::eCommandBufferBeginInfo;
		begin.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;*/

		//command.begin(begin);

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

		_device.freeCommandBuffers(_commandPool, command);
	}

	static void CopyBufferToImage(vk::Device _device, vk::CommandPool _commandPool, vk::Queue _queue, vk::Buffer& _src, vk::Image& _dst, vk::Extent2D _size)
	{
		vk::CommandBuffer commandBuffer = BeginSingleUseCommand(_commandPool, _device);

		vk::BufferImageCopy copyCommand;
		copyCommand.bufferOffset = 0;
		copyCommand.bufferRowLength = 0;
		copyCommand.bufferImageHeight = 0;

		copyCommand.imageOffset = vk::Offset3D{ 0,0,0 };
		copyCommand.imageExtent = vk::Extent3D{ _size.width, _size.height, 1 };

		copyCommand.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
		copyCommand.imageSubresource.mipLevel = 0;
		copyCommand.imageSubresource.layerCount = 1;
		copyCommand.imageSubresource.baseArrayLayer = 0;

		commandBuffer.copyBufferToImage(_src, _dst, vk::ImageLayout::eTransferDstOptimal, copyCommand);

		EndSingleUseCommandBuffer(commandBuffer, _queue, _device, _commandPool);
	}

	static void CreateBuffer(vk::Device _device, vk::PhysicalDevice _physDevice, BufferCreateInfo _info)
	{
		vk::BufferCreateInfo bufferInfo;
		bufferInfo.sType = vk::StructureType::eBufferCreateInfo;
		bufferInfo.size = _info.Size;
		bufferInfo.usage = _info.usage;
		bufferInfo.sharingMode = vk::SharingMode::eExclusive;

		_info.buffer = _device.createBuffer(bufferInfo);

		vk::MemoryRequirements memoryReqs;
		memoryReqs = _device.getBufferMemoryRequirements(_info.buffer);

		vk::MemoryAllocateInfo memoryInfo;
		memoryInfo.sType = vk::StructureType::eMemoryAllocateInfo;
		memoryInfo.allocationSize = memoryReqs.size > 0 ? memoryReqs.size : vk::WholeSize;
		memoryInfo.memoryTypeIndex = VulkanHelperFunctions::FindMemoryTypeIndex(memoryReqs.memoryTypeBits, _info.properties, _physDevice);
		_info.memory = _device.allocateMemory(memoryInfo);
		_device.bindBufferMemory(_info.buffer, _info.memory, 0);
	}

	static void CreateBufferWithStaging(vk::Device _device, vk::PhysicalDevice _physDevice, vk::CommandPool _commandPool, vk::Queue _graphicsQueue, BufferCreateInfo _info, void* _data)
	{
		vk::Buffer stagingBuffer;
		vk::DeviceMemory stagingMemory;

		BufferCreateInfo stagingBufferInfo =
		{
			.buffer = stagingBuffer,
			.memory = stagingMemory,
			.usage = vk::BufferUsageFlagBits::eTransferSrc,
			.properties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
			.Size = _info.Size
		};

		CreateBuffer(_device, _physDevice, stagingBufferInfo);
		void* stagingMap = _device.mapMemory(stagingBufferInfo.memory, 0, _info.Size);
		memcpy(stagingMap, _data, _info.Size);

		//_info.usage |= vk::BufferUsageFlagBits::eTransferDst;
		CreateBuffer(_device, _physDevice, _info);
		CopyBufferToBuffer(_device, _commandPool, stagingBuffer, _info.buffer, _info.Size, _graphicsQueue);

		_device.unmapMemory(stagingMemory);
		_device.freeMemory(stagingMemory);
		_device.destroyBuffer(stagingBuffer);
		stagingMap = nullptr;
	}

	static vk::ShaderModule WrapShader(vk::Device _device, void* _shaderBytes, size_t _shaderCodeSize)
	{
		vk::ShaderModuleCreateInfo info{};
		info.sType = vk::StructureType::eShaderModuleCreateInfo;
		info.codeSize = _shaderCodeSize;
		info.pCode = reinterpret_cast<const uint32_t*>(_shaderBytes);

		return 	_device.createShaderModule(info);
	}

	static vk::SurfaceFormatKHR GetFormat(vk::PhysicalDevice _physDevice, std::vector<vk::SurfaceFormatKHR>& _format)
	{
		for (int i = 0; i < _format.size(); i++)
		{
			if (_format[i].format != vk::Format::eR16G16B16A16Sfloat)
				continue;
			auto properties = _physDevice.getFormatProperties(_format[i].format);
			auto value = properties.linearTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment;
			if ((properties.linearTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment) == vk::FormatFeatureFlagBits::eDepthStencilAttachment)
				return _format[i];
		}

		return _format[0];
	}

	static vk::PresentModeKHR GetPresentMode(std::vector<vk::PresentModeKHR>& _modes)
	{
		if (std::find(_modes.begin(), _modes.end(), vk::PresentModeKHR::eMailbox) != _modes.end())
			return vk::PresentModeKHR::eMailbox;

		return vk::PresentModeKHR::eFifo;
	}
};

using vhf = VulkanHelperFunctions;

#endif