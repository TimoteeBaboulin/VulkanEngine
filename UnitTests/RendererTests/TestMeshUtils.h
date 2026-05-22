#pragma once

#include "Engine/Renderer/Renderer.h"
#include "Engine/ResourceManagement/Banks/MeshBank.h"

inline Moonlit::Renderer::MeshHandle CreateTestCubeMesh()
{
    static const char* kName = "TestCube";
    auto& bank = Moonlit::ResourceManagement::MeshBank::Get();
    if (bank.Exist(kName))
        return bank.GetResource(kName);

    static Moonlit::Vertex vertices[8] = {
        { {-0.5f, -0.5f, -0.5f} }, { { 0.5f, -0.5f, -0.5f} },
        { { 0.5f,  0.5f, -0.5f} }, { {-0.5f,  0.5f, -0.5f} },
        { {-0.5f, -0.5f,  0.5f} }, { { 0.5f, -0.5f,  0.5f} },
        { { 0.5f,  0.5f,  0.5f} }, { {-0.5f,  0.5f,  0.5f} }
    };
    static uint16_t cubeIndices[36] = {
        0,1,2, 2,3,0,
        4,5,6, 6,7,4,
        0,4,7, 7,3,0,
        1,5,6, 6,2,1,
        3,2,6, 6,7,3,
        0,1,5, 5,4,0
    };

    Moonlit::MeshData mesh;
    mesh.vertexCount = 8;
    mesh.vertices = vertices;
    mesh.triangleCount = 12;
    mesh.indices = cubeIndices;

    return bank.Insert(kName, mesh);
}

inline Moonlit::Renderer::MeshHandle CreateTestTriangleMesh()
{
    static const char* kName = "TestTriangle";
    auto& bank = Moonlit::ResourceManagement::MeshBank::Get();
    if (bank.Exist(kName))
        return bank.GetResource(kName);

    static Moonlit::Vertex vertices[3] = {
        { { 0.0f,  0.5f, 0.0f} },
        { { 0.5f, -0.5f, 0.0f} },
        { {-0.5f, -0.5f, 0.0f} }
    };
    static uint16_t triIndices[3] = { 0, 1, 2 };

    Moonlit::MeshData mesh;
    mesh.vertexCount = 3;
    mesh.vertices = vertices;
    mesh.triangleCount = 1;
    mesh.indices = triIndices;

    return bank.Insert(kName, mesh);
}
