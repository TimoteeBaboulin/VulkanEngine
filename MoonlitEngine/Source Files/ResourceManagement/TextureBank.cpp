#include "ResourceManagement/TextureBank.h"

#include <filesystem>
#include <stdexcept>

#include "common.h"
#include "Engine/Renderer/VulkanHelperFunctions.h"
#include "ResourceManagement/ImageHelper.h"

//ResourceBank<Image>* TextureBank::Instance = new TextureBank();

bool TextureBank::TryLoad(std::string _filepath)
{
    namespace fs = std::filesystem;

    fs::path path(_filepath);
    if (!fs::is_regular_file(path))
        throw new std::runtime_error(_filepath + " is not a regular file");
    std::string name = path.stem().string();
	std::shared_ptr<Image> texturePtr = std::make_shared<Image>();
	

    if (Exist(name))
    {
        std::cout << "[MeshBank] Mesh with name " << name << " already exist in MeshBank." << std::endl;
        return false;
    }

    *texturePtr.get() = ImageHelper::LoadFromFile(_filepath);

	m_resources.push_back(ResourcePair<Image>{
		name,
		texturePtr
	});

    return true;
}

//TextureData TextureBank::GetTextureData(Image _image)
//{
//	vk::Buffer stagingBuffer;
//	vk::Device device = VulkanEngine::LogicalDevice;
//	vk::DeviceMemory stagingMemory;
//	vk::DeviceMemory imageMemory;
//	size_t memorySize = _image.width * _image.height * 4;
//	vk::Extent2D extent = { _image.width, _image.height };
//
//	TextureData texData;
//
//	//Create staging buffer
//	BufferCreateInfo staging = {
//		.buffer = stagingBuffer,
//		.memory = stagingMemory,
//		.usage = vk::BufferUsageFlagBits::eTransferSrc,
//		.properties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
//		.size = memorySize
//	};
//
//	vhf::CreateBuffer(staging);
//	void* map = device.mapMemory(stagingMemory, 0, memorySize);
//	memcpy(map, _image.pixels, memorySize);
//
//	//Create Image
//	vhf::CreateImage(extent, vk::Format::eR8G8B8A8Srgb, vk::ImageTiling::eOptimal,
//		vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal,
//		texData.m_image, imageMemory, vk::ImageLayout::eUndefined);
//
//	TransitionInfo transInfo =
//	{
//		.srcAccessFlags = vk::AccessFlagBits::eNone,
//		.dstAccessFlags = vk::AccessFlagBits::eTransferWrite,
//		.srcStage = vk::PipelineStageFlagBits::eTopOfPipe,
//		.dstStage = vk::PipelineStageFlagBits::eTransfer
//	};
//	vhf::TransitionImageLayout(texData.m_image, vk::Format::eR8G8B8A8Srgb, vk::ImageAspectFlagBits::eColor, vk::ImageLayout::eUndefined,
//		vk::ImageLayout::eTransferDstOptimal, transInfo);
//
//	//Copy staging buffer to image
//	vhf::CopyBufferToImage(stagingBuffer, texData.m_image, extent);
//
//	//Prepare for shader read
//	transInfo =
//	{
//		.srcAccessFlags = vk::AccessFlagBits::eTransferWrite,
//		.dstAccessFlags = vk::AccessFlagBits::eShaderRead,
//		.srcStage = vk::PipelineStageFlagBits::eTransfer,
//		.dstStage = vk::PipelineStageFlagBits::eFragmentShader
//	};
//
//	vhf::TransitionImageLayout(texData.m_image, vk::Format::eR8G8B8A8Srgb, vk::ImageAspectFlagBits::eColor, vk::ImageLayout::eTransferDstOptimal,
//		vk::ImageLayout::eShaderReadOnlyOptimal, transInfo);
//
//	//std::vector<vk::ImageView> imageViews(1);
//	texData.m_imageView = vhf::CreateImageView(texData.m_image, vk::Format::eR8G8B8A8Srgb, vk::ImageAspectFlagBits::eColor);
//
//	//std::vector<vk::Sampler> samplers(1);
//
//	//Create sampler
//	vk::SamplerCreateInfo info;
//	info.sType = vk::StructureType::eSamplerCreateInfo;
//	info.magFilter = vk::Filter::eLinear;
//	info.minFilter = vk::Filter::eLinear;
//
//	info.addressModeU = vk::SamplerAddressMode::eRepeat;
//	info.addressModeV = vk::SamplerAddressMode::eRepeat;
//	info.addressModeW = vk::SamplerAddressMode::eRepeat;
//
//	info.anisotropyEnable = false;
//	info.maxAnisotropy = 4;
//	info.borderColor = vk::BorderColor::eIntOpaqueBlack;
//	info.unnormalizedCoordinates = false;
//
//	info.compareEnable = false;
//	info.compareOp = vk::CompareOp::eAlways;
//
//	info.mipmapMode = vk::SamplerMipmapMode::eLinear;
//	info.mipLodBias = 0.0f;
//	info.minLod = 0.0f;
//	info.maxLod = 0.0f;
//
//	texData.m_sampler = VulkanEngine::LogicalDevice.createSampler(info);
//
//	//Free staging buffer
//	device.unmapMemory(stagingMemory);
//	device.destroyBuffer(stagingBuffer);
//
//    return texData;
//}
