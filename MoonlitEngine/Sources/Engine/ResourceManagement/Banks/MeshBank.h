#pragma once

#include "../ResourceBank.h"
#include "common.h"

namespace Moonlit::ResourceManagement
{
	class MOONLIT_API MeshBank : public ResourceBank<MeshData>
	{
	public:
		static void Initialize()
		{
			if (Instance == nullptr)
				Instance = new MeshBank();
		}

		static MeshBank& Get()
		{
			if (Instance == nullptr)
				Initialize();
			return *static_cast<MeshBank*>(Instance);
		}

		MeshBank() : ResourceBank<MeshData>() {};

		bool TryLoad(const std::string& _filepath) override;

		HANDLE_TYPE Insert(std::string _name, MeshData _data)
		{
			InsertResource(_name, _data);
			return GetResource(_name);
		}
	};
}
