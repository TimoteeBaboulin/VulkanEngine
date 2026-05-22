#pragma once

#include "common.h"
#include "../CustomVulkanStructs.h"

#include <vector>
#include <string>

namespace Moonlit
{
	class MaterialInstance;
	namespace Renderer
	{
		struct DeviceData;
		class RenderTarget;
	}

	struct ShaderCode
	{
		void* CodePtr;
		size_t Size;
	};

	enum ResourceType
	{
		Mat4x4 = 0,
		Vec4 = 1,
		Vec3 = 2,
		Vec2 = 3,
		Texture = 4
	};


	struct ShaderResource
	{
		ResourceType Type;
		std::string Name;
		bool IsArray;
		size_t ArraySize;
	};

	struct ShaderFunction
	{
		const char* Name;
		std::vector<ShaderResource> InParameters;
		std::vector<ShaderResource> OutParameters;

		ShaderCode Code;
	};

	struct EntryPoint
	{
		vk::ShaderStageFlagBits Stage;
		std::vector<std::string> SubpassNames;
		ShaderFunction Function;
	};

	// TODO: Make it handle resources (textures, buffers, etc.) and their bindings.
	struct ShaderData
	{
		std::vector<EntryPoint> EntryPoints;
		std::vector<ShaderResource> GlobalResources;
	};

	/// <summary>
	/// Class representing a shader module <para/>
	/// This class is device agnostic
	/// </summary>
	class MOONLIT_API Material
	{
		friend class MaterialInstance;

	public:
		Material(std::string _shaderPath);
		~Material();

		int GetTextureCount() const { return m_textureCount; }
		ShaderData GetShaderData() const;
		std::vector<std::string> GetIncludedSubpasses() const { return m_includedSubpasses; }

	private:
		std::string m_shaderPath;
		ShaderData m_shaderData;

		std::vector<std::string> m_includedSubpasses;

		int m_textureCount = 0;
	};
}