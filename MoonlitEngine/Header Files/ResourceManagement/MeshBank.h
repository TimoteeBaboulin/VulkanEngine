#pragma once

#include "ResourceBank.h"
#include "common.h"

class MeshBank : public ResourceBank<MeshData>
{
public:
	bool TryLoad(std::string _filepath) override;
};