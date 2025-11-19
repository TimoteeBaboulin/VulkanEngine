#pragma once

#include "common.h"
#include <vector>

class MaterialInstance;
class RenderTarget;

struct ShaderCode
{
	void* code;
	size_t size;
};

enum ResourceType
{
	Mat4x4 = 0,
	Vec4 = 1,
	Vec3 = 2,
	Vec2 = 3,
	Texture = 4
};

struct ShaderFunction
{
	const char* Name;
	std::vector<ShaderResource> InParameters;
	std::vector<ShaderResource> OutParameters;

	ShaderCode* Code;
};

struct ShaderResource
{
	ResourceType Type;
	const char* Name;
	bool IsArray;
	size_t ArraySize;
};

struct ShaderData
{
	std::vector<ShaderFunction> EntryPoints;
	std::vector<ShaderResource> GlobalResources;
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