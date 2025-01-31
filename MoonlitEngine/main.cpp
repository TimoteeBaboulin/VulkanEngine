#define GLFW_EXPOSE_NATIVE_WIN32

#include "MoonlitVulkan.h"
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"


constexpr int WindowWidth = 1920;
constexpr int WindowHeight = 1080;

void main()
{
	VulkanEngine engine;
    std::vector<std::pair<int, int>> hints;
    hints = { {GLFW_CLIENT_API, GLFW_NO_API}, {GLFW_RESIZABLE, GLFW_FALSE} };
    glfwInit();
    for (std::pair<int, int> pair : hints)
    {
        glfwWindowHint(pair.first, pair.second);
    }
    GLFWwindow* window = glfwCreateWindow(WindowWidth, WindowHeight, "Vulkan Engine", nullptr, nullptr);;
    unsigned int extensionCount;
    const char** extensions = glfwGetRequiredInstanceExtensions(&extensionCount);

    ContextInfo context = {
        .name = "Vulkan Engine",
        .width = WindowWidth,
        .height = WindowHeight,
        .windowHandle = glfwGetWin32Window(window)
    };

    engine.InitContext(context, extensions, extensionCount);
    engine.InitVulkan();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        engine.Render();
    }
}