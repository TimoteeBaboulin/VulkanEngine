#pragma once

#include <vector>
#define VK_USE_PLATFORM_WIN32_KHR
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan.h"
#include "MoonlitVulkanExport.h"
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

class MOONLIT_API VulkanContext
{
public:
	static bool ValidationLayersEnabled;

	void Init(const ContextInfo& _info);
	void Cleanup();

	vk::Instance CreateInstance(vk::ApplicationInfo _appInfo, const char** requiredExtensions, int extensionCount);
	void CreateSurfaceKHR();
	/*bool ShouldClose() const;
	void PollEvents() const;*/

	vk::Extent2D GetSurfaceExtent(vk::PhysicalDevice physicalDevice);
	vk::Extent2D GetExtent(vk::SurfaceCapabilitiesKHR _capabilities);
	HWND GetWindow() const { return m_window; }
	vk::SurfaceKHR* GetSurface() { return &m_surface; }

private:
	HWND m_window = nullptr;
	vk::Instance m_instance;
	vk::Extent2D m_extent;
	std::string m_windowName;

	vk::SurfaceKHR m_surface;
};