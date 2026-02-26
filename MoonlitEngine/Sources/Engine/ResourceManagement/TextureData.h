#pragma once

#include "../Renderer/CustomVulkanStructs.h"

namespace Moonlit
{
	struct TextureData
	{
		vk::ImageView m_imageView;
		vk::Image m_image;

		vk::Sampler m_sampler;
	};
}