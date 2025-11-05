#pragma once

#include "Engine/Renderer/CustomVulkanStructs.h"

struct TextureData
{
	vk::ImageView m_imageView;
	vk::Image m_image;

	vk::Sampler m_sampler;
};