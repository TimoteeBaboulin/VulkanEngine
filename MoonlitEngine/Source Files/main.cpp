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

constexpr int WindowWidth = 1920;
constexpr int WindowHeight = 1080;

MeshData GetMesh(aiMesh* _mesh);
MeshData ImportMesh(std::string _path);
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
    MeshData mesh = ImportMesh("Meshes/barstool.gltf");
    mesh.textures.push_back(Image());
    ImportImage("Textures/barstool_albedo.png", mesh.textures[0]);
	GLFWwindow* window = InitWindow(app);

    app.InitVulkan(); 
    app.LoadMesh(mesh);

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

MeshData ImportMesh(std::string _path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(_path, aiPostProcessSteps::aiProcess_OptimizeMeshes | aiPostProcessSteps::aiProcess_Triangulate | aiPostProcessSteps::aiProcess_FlipUVs);
    if (scene == nullptr)
    {
        std::string errorMessage = importer.GetErrorString();
        throw new std::runtime_error(errorMessage);
    }
    MeshData mesh = GetMesh(scene->mMeshes[0]);
    return mesh;
}
MeshData GetMesh(aiMesh* _mesh)
{
    MeshData mesh;
    mesh.vertexCount = _mesh->mNumVertices;
    mesh.vertices = new Vertex[mesh.vertexCount];

    for (int i = 0; i < _mesh->mNumVertices; i++)
    {
        mesh.vertices[i] = Vertex(_mesh->mVertices[i].x, _mesh->mVertices[i].y, _mesh->mVertices[i].z);
        
        mesh.vertices[i].uvX = _mesh->mTextureCoords[0][i].x;
        mesh.vertices[i].uvY = _mesh->mTextureCoords[0][i].y;
    }
    mesh.triangleCount = _mesh->mNumFaces;
    mesh.indices = new int[mesh.triangleCount * 3];

    for (int i = 0; i < mesh.triangleCount; i++)
    {
        mesh.indices[i * 3] = _mesh->mFaces[i].mIndices[0];
        mesh.indices[i * 3 + 1] = _mesh->mFaces[i].mIndices[1];
        mesh.indices[i * 3 + 2] = _mesh->mFaces[i].mIndices[2];
    }

    return mesh;
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
    const char** extensions = glfwGetRequiredInstanceExtensions(&extensionCount);

    HWND winHandle = glfwGetWin32Window(window);

    ContextInfo context = {
        .name = "Vulkan Engine",
        .width = WindowWidth,
        .height = WindowHeight,
        .windowHandle = winHandle
    };

    _app.InitContext(context, extensions, extensionCount);
    return window;
}