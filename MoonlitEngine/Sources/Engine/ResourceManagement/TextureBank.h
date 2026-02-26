#pragma once

#include "ResourceBank.h"
#include "Resources/Image.h"

#include "common.h"

namespace Moonlit::ResourceManagement
{
	class MOONLIT_API TextureBank : public ResourceBank<Moonlit::Image>
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
}