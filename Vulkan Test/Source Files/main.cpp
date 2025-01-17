#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <iostream>
#include "MoonlitVulkan/VulkanEngine.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/mesh.h"

Mesh GetMesh(aiMesh* _mesh)
{
    Mesh mesh;
    mesh.vertexCount = _mesh->mNumVertices;
    mesh.vertices = new float[mesh.vertexCount * 3];

    for (int i = 0; i < _mesh->mNumVertices * 3; i++)
    {
        mesh.vertices[i * 3] = _mesh->mVertices[i].x;
        mesh.vertices[i * 3 + 1] = _mesh->mVertices[i].y;
        mesh.vertices[i * 3 + 2] = _mesh->mVertices[i].z;
    }
    mesh.triangleCount = _mesh->mNumFaces;
    mesh.indices = new int[mesh.triangleCount * 3];

    for (int i = 0; i < mesh.triangleCount; i++)
    {
        mesh.indices[i * 3    ] = _mesh->mFaces[i].mIndices[0];
        mesh.indices[i * 3 + 1] = _mesh->mFaces[i].mIndices[1];
        mesh.indices[i * 3 + 2] = _mesh->mFaces[i].mIndices[2];
    }

    return mesh;
}

int main() {
    VulkanEngine app;
    Assimp::Importer importer;
    importer.ReadFile("./Assets/shull.fbx", aiPostProcessSteps::aiProcess_Triangulate);
    const aiScene* scene = importer.GetScene();
    Mesh mesh = GetMesh(scene->mMeshes[0]);
    
    app.InitContext();
    app.InitVulkan(); 
    app.LoadMesh(mesh);

    GLFWwindow* window = app.GetWindow();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        app.Render();
    }

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