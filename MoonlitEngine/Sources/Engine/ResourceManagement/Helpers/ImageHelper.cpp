#include "Engine/ResourceManagement/Helpers/ImageHelper.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <string>
#include <stdexcept>

Moonlit::Image Moonlit::ImageHelper::LoadFromFile(std::string _path)
{
	Image image;

	stbi_info(_path.c_str(), (int*)&image.width, (int*)&image.height, &image.channels);

	int width = image.width;
	int height = image.height;
	image.pixels = stbi_load(_path.c_str(), &width, &height, &image.channels, STBI_rgb_alpha);

	if (image.pixels == nullptr)
	{
		throw std::runtime_error("Failed to load image");
	}

	return image;
}
