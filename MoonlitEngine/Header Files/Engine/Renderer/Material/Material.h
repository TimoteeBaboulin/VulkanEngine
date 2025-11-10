#pragma once

#include "common.h"

#include <string>

class MaterialInstance;
class RenderTarget;

struct ShaderCode
{
	void* code;
	size_t size;
};

/// <summary>
/// Class representing a shader module <para/>
/// This class is device agnostic
/// </summary>
class Material
{
	friend class MaterialInstance;

public:
	Material(std::string _shaderPath);
	~Material();

	MaterialInstance* CreateInstance(RenderTarget& _target);

private:
	void RemoveInstance(MaterialInstance* _instance);

	std::string m_shaderPath;
	ShaderCode* m_shaderCode;

	std::vector<MaterialInstance*> m_instances;
};