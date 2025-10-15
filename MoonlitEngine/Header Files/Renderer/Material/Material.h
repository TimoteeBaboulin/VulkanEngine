#pragma once

#include "common.h"

class MaterialInstance;
class RenderTarget;

struct ShaderCode
{
	void* code;
	size_t size;
};

class Material
{
public:
	Material(std::string _shaderPath);
	~Material();

	MaterialInstance* CreateInstance(RenderTarget& _target);

private:
	std::string m_shaderPath;
	ShaderCode* m_shaderCode;

	std::vector<MaterialInstance*> m_instances;
};