#pragma once

#include "ResourceBank.h"
#include "TextureData.h"

#include "common.h"

class MOONLIT_API TextureBank : public ResourceBank<Image>
{
public:
	static void Initialize()
	{
		if (Instance == nullptr)
			Instance = new TextureBank();
	}

	static TextureBank* GetInstance()
	{
		if (Instance == nullptr)
			Initialize();
		return static_cast<TextureBank*>(Instance);
	}

	bool TryLoad(std::string _filepath) override;

private:
};