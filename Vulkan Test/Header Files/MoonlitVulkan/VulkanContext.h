#pragma once

#define GLFW_INCLUDE_VULKAN

#include <vector>

#include "vulkan/vulkan.hpp"

#include "MoonlitVulkanExport.h"

#include "Windows.h"

struct ContextInfo
{
		const char* name;
		uint32_t width;
		uint32_t height;
};

class MOONLIT_API VulkanContext
{
public:
	static bool ValidationLayersEnabled;

	void Init(const ContextInfo& _info);
	void Cleanup();

	vk::Instance CreateInstance(vk::ApplicationInfo _appInfo, const char** requiredExtensions, int extensionCount);
	void CreateSurfaceKHR(vk::SurfaceKHR* _out);
	bool ShouldClose() const;
	void PollEvents() const;

	vk::Extent2D GetExtent(vk::SurfaceCapabilitiesKHR _capabilities) const;
	HWND* GetWindow() const { return m_window; }

private:
	HWND* m_window = nullptr;
	vk::Instance m_instance;
	vk::Extent2D m_extent;
	std::string m_windowName;
};