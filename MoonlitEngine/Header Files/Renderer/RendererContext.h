#pragma once

#include <vector>
#define VK_USE_PLATFORM_WIN32_KHR
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan.h"
#include "MoonlitExport.h"
#include "Windows.h"
#undef MemoryBarrier
#undef max

struct ContextInfo
{
		const char* name;
		uint32_t width;
		uint32_t height;

		HWND windowHandle;
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