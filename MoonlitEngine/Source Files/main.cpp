#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL

#define VK_USE_PLATFORM_WIN32_KHR

#define WIN32_LEAN_AND_MEAN

#ifndef EDITOR_BUILD

#include <Windows.h>

#undef MemoryBarrier
#undef max

#include <vulkan/vulkan.hpp>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qwidget.h>
#include <QtCore/qtimer.h>
#include <QtWidgets/qmainwindow.h>
#include "Engine/Inputs/InputManager.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/MoonlitEngine.h"
#include "Camera.h"
#include "Debug/Logger.h"

constexpr int WindowWidth = 960;
constexpr int WindowHeight = 540;

GLFWwindow* InitWindow(Renderer& _app);
void ImportImage(std::string _path, Image& _image);

void MousePressedCallback(MOUSE_KEY _key)
{
    if (_key == MOUSE_KEY::LEFT_CLICK)
    {
		Logger::LogInfo("Left Mouse Clicked");
    }
}

void GamepadPressedCallback(GAMEPAD_KEY _key)
{
	if (_key == GAMEPAD_KEY::BUTTON_A)
	{
        Logger::LogInfo("A pressed");
	}
}

void GamepadAxisCallback(GAMEPAD_KEY _key, float _x, float _y)
{
	std::string text = "Axis value is " + std::to_string(_x) + ", " + std::to_string(_y);
    Logger::LogInfo(text.c_str());
}

int main(int argc, char** argv) 
{
    QApplication* application = new QApplication(argc, argv);
    Camera* camera = new Camera(glm::vec3(20.0f, 30.0f, 35.0f), glm::vec3(1.0, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    QMainWindow* window = new QMainWindow();
    QWidget* widget = new QWidget(window);
    window->resize(WindowWidth, WindowHeight);
    window->show();

    HWND winHandle = (HWND)window->effectiveWinId();

    MoonlitEngine engine(winHandle);
	engine.AddRenderTarget((void*)winHandle, camera);

    InputManager::GetInstance()->LinkQtApplication(application);
    QTimer* timer = new QTimer(0);
    timer->setSingleShot(false);
    std::function<void (void)> updateFunction = std::bind(&MoonlitEngine::Update, &engine);
	timer->connect(timer, &QTimer::timeout, updateFunction);
    timer->start();

    engine.Init();

    InputManager::GetInstance()->SubscribeMouseEvent(KEY_STATE::PRESSED, MousePressedCallback);
    InputManager::GetInstance()->SubscribeGamepadEvent(KEY_STATE::PRESSED, GamepadPressedCallback);
    InputManager::GetInstance()->SubscribeGamepadAxisEvent(GamepadAxisCallback);

    application->processEvents();

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
        .name = "Vulkan Engine"
    };

    _app.Init(context, extensions);
    return window;
}
#endif // !EDITOR_BUILD