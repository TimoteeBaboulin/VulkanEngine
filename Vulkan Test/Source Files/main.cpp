#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <iostream>
#include "MoonlitVulkan/VulkanEngine.h"

int main() {
    VulkanEngine app;

    try
    {
        app.Run();
    }
    catch (const std::exception& err)
    {
        std::cerr << err.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}