#pragma once

// Only place where vulkan.hpp is included
// This .h should not be included anywhere but in CustomVulkanStructs.h

// Undefine MemoryBarrier to avoid conflits with Windows.h

#ifndef VK_USE_PLATFORM_WIN32_KHR
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#undef MemoryBarrier
#undef ImageMemoryBarrier
#undef BufferMemoryBarrier
#include <vulkan/vulkan.hpp>


struct VulkanTest {
    vk::MemoryBarrier memoryBarrier;
};

