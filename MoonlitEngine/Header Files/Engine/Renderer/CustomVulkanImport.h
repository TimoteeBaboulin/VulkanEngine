#ifndef CUSTOM_VULKAN_IMPORT_H
#define CUSTOM_VULKAN_IMPORT_H

// Only place where vulkan.hpp is included
// This .h should not be included anywhere but in CustomVulkanStructs.h

// Undefine MemoryBarrier to avoid conflits with Windows.h

#undef MemoryBarrier
#include <vulkan/vulkan.hpp>

#endif // !CUSTOM_VULKAN_IMPORT_H
