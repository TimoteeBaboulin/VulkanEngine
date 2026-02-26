#pragma once

#include "ResourceBank.h"
#include "../../common.h"

namespace Moonlit::ResourceManagement
{
	class MOONLIT_API MeshBank : public ResourceBank<Moonlit::MeshData>
	{
	public:
		static void Initialize()
		{
			if (Instance == nullptr)
				Instance = new MeshBank();
		}

		static MeshBank* GetInstance()
		{
			if (Instance == nullptr)
				Initialize();
			return static_cast<MeshBank*>(Instance);
		}

		bool TryLoad(std::string _filepath) override;
	};
}
