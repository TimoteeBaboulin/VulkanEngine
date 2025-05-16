#pragma once

#include "ResourceBank.h"
#include "TextureData.h"

#include "common.h"

class TextureBank : public ResourceBank<TextureData>
{
public:
	bool TryLoad(std::string _filepath) override;

private:
	TextureData GetTextureData(Image _image);
};