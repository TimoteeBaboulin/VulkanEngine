#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#define VK_USE_PLATFORM_WIN32_KHR

#define WIN32_LEAN_AND_MEAN


#ifndef EDITOR_BUILD

#include "Windows.h"

#undef MemoryBarrier
#undef max

#include <vulkan/vulkan.hpp>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <iostream>
#include "Inputs/InputManager.h"

#include <algorithm>
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/mesh.h"

#include "Renderer/Renderer.h"
#include "Engine/MoonlitEngine.h"

#include "ResourceManagement/MeshBank.h"

#include "QtWidgets/qapplication.h"
#include "QtWidgets/qwidget.h"
#include "QtCore/qtimer.h"
#include "ResourceManagement/TextureBank.h"

constexpr int WindowWidth = 1920;
constexpr int WindowHeight = 1080;

GLFWwindow* InitWindow(Renderer& _app);
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

int main(int argc, char** argv) 
{
    QApplication* application = new QApplication(argc, argv);
    QWidget* window = new QWidget();
    window->resize(WindowWidth, WindowHeight);
    window->show();
    
    HWND winHandle = (HWND)window->effectiveWinId();

    MoonlitEngine engine(winHandle);
    QTimer* timer = new QTimer(0);
    timer->setSingleShot(false);
    std::function<void (void)> updateFunction = std::bind(&MoonlitEngine::Update, &engine);
	timer->connect(timer, &QTimer::timeout, updateFunction);
    //timer->setParent(application);
    timer->start();

    engine.Init();

    InputManager::GetInstance()->SubscribeMouseEvent(KEY_STATE::PRESSED, MousePressedCallback);
    InputManager::GetInstance()->SubscribeGamepadEvent(KEY_STATE::PRESSED, GamepadPressedCallback);
    InputManager::GetInstance()->SubscribeGamepadAxisEvent(GamepadAxisCallback);

    return application->exec();
}

GLFWwindow* InitWindow(Renderer& _app)
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
    InputManager::InitManager(winHandle);
    ContextInfo context = {
        .name = "Vulkan Engine",
        .width = WindowWidth,
        .height = WindowHeight,
        .windowHandle = winHandle
    };

    _app.Init(context, extensions);
    return window;
}
#endif // !EDITOR_BUILD