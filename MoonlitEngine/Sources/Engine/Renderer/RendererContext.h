#pragma once

#include "MoonlitExport.h"
#include "Engine/Renderer/CustomVulkanImport.h"

namespace Moonlit::Renderer
{

	struct ContextInfo
	{
		const char* name;
	};

	class MOONLIT_API RendererContext
	{
	public:
		static bool ValidationLayersEnabled;

		~RendererContext();

		void Init();
		void Cleanup();

		vk::Instance* CreateInstance(vk::ApplicationInfo _appInfo, const char** requiredExtensions, int extensionCount);

	private:
		vk::Instance m_instance;
	};
}