#pragma once
#define VK_USE_PLATFORM_WIN32_KHR

#include <vulkan/vulkan.hpp>
#include <Windows.h>

#undef MemoryBarrier
#undef max
#include "MoonlitExport.h"


struct ContextInfo
{
		const char* name;
};

class MOONLIT_API RendererContext
{
public:
	static bool ValidationLayersEnabled;

	void Init();
	void Cleanup();

	vk::Instance CreateInstance(vk::ApplicationInfo _appInfo, const char** requiredExtensions, int extensionCount);

private:
	vk::Instance m_instance;
};