#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE


#define WIN32_LEAN_AND_MEAN
#define STB_IMAGE_IMPLEMENTATION

#define NOMINMAX

#include <vulkan/vulkan.hpp>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <iostream>
#include "Inputs/InputManager.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/mesh.h"

#include "Renderer/VulkanEngine.h"
#include "Renderer/ContextManager.h"
#include "Renderer/MeshInstanceManager.h"

#include "ResourceManagement/MeshBank.h"

constexpr int WindowWidth = 1920;
constexpr int WindowHeight = 1080;

GLFWwindow* InitWindow(VulkanEngine& _app);
void ImportImage(std::string _path, Image& _image);

void MousePressedCallback(MOUSE_KEY _key)
{
    if (_key == MOUSE_KEY::LEFT_CLICK)
    {
        std::cout << "Left Click" << std::endl;
    }
}

void GamepadPressedCallback(GAMEPAD_KEY _key)
{
	if (_key == GAMEPAD_KEY::BUTTON_A)
	{
		std::cout << "A Pressed" << std::endl;
	}
}

void GamepadAxisCallback(GAMEPAD_KEY _key, float _x, float _y)
{
    std::cout << "Axis value is " << _x << ", " << _y << std::endl;
}

int main() 
{
    VulkanEngine app;
    Image texture;
    ImportImage("Textures/barstool_albedo.png", texture);
	GLFWwindow* window = InitWindow(app);

    MeshBank::Instance->TryLoad("Meshes/barstool.gltf");

    app.InitVulkan(); 
    app.LoadMesh("barstool");

    MeshInstanceManager* manager = MeshInstanceManager::Get();

    InputManager::GetInstance()->SubscribeMouseEvent(KEY_STATE::PRESSED, MousePressedCallback);
    InputManager::GetInstance()->SubscribeGamepadEvent(KEY_STATE::PRESSED, GamepadPressedCallback);
    InputManager::GetInstance()->SubscribeGamepadAxisEvent(GamepadAxisCallback);
	InputManager::GetInstance()->SubscribeKeyboardEvent(KEY_STATE::PRESSED, [window](KEYBOARD_KEY _key) {
		if (_key == KEYBOARD_KEY::R)
		{
			glfwSetWindowSize(window, WindowWidth - 1, WindowHeight - 2);
		}
		});

    while (!glfwWindowShouldClose(window))
    {
        InputManager::GetInstance()->PollEvents();

        if (!glfwWindowShouldClose(window))
        {
            app.Render();
        }
        
        glfwPollEvents();
    }

    return EXIT_SUCCESS;
}

GLFWwindow* InitWindow(VulkanEngine& _app)
{
    std::vector<std::pair<int, int>> hints;
    hints = { {GLFW_CLIENT_API, GLFW_NO_API}, {GLFW_RESIZABLE, GLFW_TRUE} };
    glfwInit();
    for (std::pair<int, int> pair : hints)
    {
        glfwWindowHint(pair.first, pair.second);
    }
    GLFWwindow* window = glfwCreateWindow(WindowWidth, WindowHeight, "Vulkan Engine", nullptr, nullptr);;
    unsigned int extensionCount;
    
    const char** extensionsBuffer = glfwGetRequiredInstanceExtensions(&extensionCount);
    std::vector<const char*> extensions;
    for (int i = 0; i < extensionCount; i++)
    {
        extensions.push_back(extensionsBuffer[i]);
    }


    HWND winHandle = glfwGetWin32Window(window);

    ContextInfo context = {
        .name = "Vulkan Engine",
        .width = WindowWidth,
        .height = WindowHeight,
        .windowHandle = winHandle
    };

    _app.InitContext(context, extensions.data(), extensionCount);
    return window;
}