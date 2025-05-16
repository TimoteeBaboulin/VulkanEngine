#pragma once

#include "vulkan/vulkan.hpp"

struct TextureData
{
	vk::ImageView m_imageView;
	vk::Image m_image;

	vk::Sampler m_sampler;
};