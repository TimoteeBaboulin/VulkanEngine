#pragma once

#define GLFW_INCLUDE_VULKAN

#include <vector>

#include "GLFW/glfw3.h"
#include "vulkan/vulkan.hpp"



struct ContextInfo
{
		const char* name;
		uint32_t width;
		uint32_t height;
		std::vector<std::pair<int, int>> hints;
};

class VulkanContext
{
public:
	void Init(const ContextInfo& _info);
	void Cleanup();

	vk::Instance CreateInstance(vk::ApplicationInfo _appInfo);
	void CreateSurfaceKHR(vk::SurfaceKHR* _out);
	bool ShouldClose() const;
	void PollEvents() const;

	vk::Extent2D GetExtent(vk::SurfaceCapabilitiesKHR _capabilities) const;

private:
	GLFWwindow* m_window = nullptr;
	vk::Instance m_instance;
};