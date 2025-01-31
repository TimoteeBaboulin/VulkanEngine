#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#define WIN32_LEAN_AND_MEAN
#define STB_IMAGE_IMPLEMENTATION

#include <vulkan/vulkan.hpp>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <iostream>
#include "MoonlitVulkan.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/mesh.h"
#include "stb_image.h"

constexpr int WindowWidth = 1920;
constexpr int WindowHeight = 1080;

struct Image
{
    stbi_uc* pixels;

    int width;
    int height;
    int channels;
};

Mesh GetMesh(aiMesh* _mesh);
Mesh ImportMesh(std::string _path);
GLFWwindow* InitWindow(VulkanEngine& _app);
void ImportImage(std::string _path, Image& _image);

void ImportImage(std::string _path, Image& _image)
{
	_image.pixels = stbi_load(_path.c_str(), &_image.width, &_image.height, &_image.channels, STBI_rgb_alpha);

	if (_image.pixels == nullptr)
	{
		throw std::runtime_error("Failed to load image");
	}
}

int main() 
{
    VulkanEngine app;
    Mesh mesh = ImportMesh("Assets/shull.fbx");
	GLFWwindow* window = InitWindow(app);
    Image image;

    ImportImage("Assets/Textures/texture.png", image);

    app.InitVulkan(); 
    app.LoadMesh(mesh);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        app.Render();
    }

    return EXIT_SUCCESS;
}

Mesh ImportMesh(std::string _path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(_path, aiPostProcessSteps::aiProcess_OptimizeMeshes | aiPostProcessSteps::aiProcess_Triangulate);
    if (scene == nullptr)
    {
        std::string errorMessage = importer.GetErrorString();
        throw new std::runtime_error(errorMessage);
    }
    Mesh mesh = GetMesh(scene->mMeshes[0]);
    return mesh;
}
Mesh GetMesh(aiMesh* _mesh)
{
    Mesh mesh;
    mesh.vertexCount = _mesh->mNumVertices;
    mesh.vertices = new Vertex[mesh.vertexCount];

    for (int i = 0; i < _mesh->mNumVertices; i++)
    {
        mesh.vertices[i] = Vertex(_mesh->mVertices[i].x, _mesh->mVertices[i].y, _mesh->mVertices[i].z);
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

    _app.InitContext(context, extensions, extensionCount);
    return window;
}