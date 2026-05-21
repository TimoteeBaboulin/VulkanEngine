#include <gtest/gtest.h>
#include <Windows.h>

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/DrawBuffer.h"
#include "Engine/Renderer/Material/Material.h"
#include "Engine/Renderer/RendererContext.h"
#include "Engine/ResourceManagement/Banks/MeshBank.h"
#include <glm/glm.hpp>

namespace
{
    Moonlit::Renderer::MeshHandle CreateTestCubeMesh()
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
        static uint16_t indices[36] = {
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
        mesh.indices = indices;

        return bank.Insert(kName, mesh);
    }

    class DrawBufferRoutingFixture : public ::testing::Test
    {
    protected:
        Moonlit::Renderer::MoonlitRenderer m_renderer;
        HWND m_hwnd = nullptr;

        void SetUp() override
        {
            WNDCLASSEX wc{};
            wc.cbSize = sizeof(WNDCLASSEX);
            wc.lpfnWndProc = DefWindowProc;
            wc.hInstance = GetModuleHandle(nullptr);
            wc.lpszClassName = "DrawBufferTestWnd";
            RegisterClassEx(&wc);
            m_hwnd = CreateWindowEx(0, "DrawBufferTestWnd", "Test", WS_OVERLAPPEDWINDOW,
                0, 0, 800, 600, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);

            Moonlit::Renderer::ContextInfo info;
            info.name = "DrawBufferRoutingTest";
            m_renderer.Init(info, {"VK_KHR_surface", "VK_KHR_win32_surface"});
            m_renderer.AddRenderTarget(m_hwnd, nullptr);
        }

        void TearDown() override
        {
            DestroyWindow(m_hwnd);
            UnregisterClass("DrawBufferTestWnd", GetModuleHandle(nullptr));
        }
    };
}

TEST_F(DrawBufferRoutingFixture, SameMaterial_ReturnsSameBuffer)
{
    Moonlit::Material mat(std::string(MATERIAL_TEST_SHADERS_DIR) + "/TestMaterial_NoTextures.slang");
    auto* buf1 = m_renderer.GetOrCreateDrawBuffer(&mat);
    auto* buf2 = m_renderer.GetOrCreateDrawBuffer(&mat);
    EXPECT_EQ(buf1, buf2);
    EXPECT_EQ(m_renderer.GetDrawBufferCount(), 1u);
}

TEST_F(DrawBufferRoutingFixture, TwoDifferentMaterials_TwoBuffers)
{
    Moonlit::Material mat1(std::string(MATERIAL_TEST_SHADERS_DIR) + "/TestMaterial_NoTextures.slang");
    Moonlit::Material mat2(std::string(MATERIAL_TEST_SHADERS_DIR) + "/TestMaterial_SingleTexture.slang");
    m_renderer.GetOrCreateDrawBuffer(&mat1);
    m_renderer.GetOrCreateDrawBuffer(&mat2);
    EXPECT_EQ(m_renderer.GetDrawBufferCount(), 2u);
}

TEST_F(DrawBufferRoutingFixture, ReusedMaterial_DoesNotAddExtraBuffer)
{
    Moonlit::Material mat1(std::string(MATERIAL_TEST_SHADERS_DIR) + "/TestMaterial_NoTextures.slang");
    Moonlit::Material mat2(std::string(MATERIAL_TEST_SHADERS_DIR) + "/TestMaterial_SingleTexture.slang");
    m_renderer.GetOrCreateDrawBuffer(&mat1);
    m_renderer.GetOrCreateDrawBuffer(&mat2);
    m_renderer.GetOrCreateDrawBuffer(&mat1);
    EXPECT_EQ(m_renderer.GetDrawBufferCount(), 2u);
}

TEST(DrawBufferVersion, StartsAtZero)
{
    Moonlit::Material mat(std::string(MATERIAL_TEST_SHADERS_DIR) + "/TestMaterial_NoTextures.slang");
    Moonlit::Renderer::DrawBuffer buf(&mat);
    EXPECT_EQ(buf.GetVersion(), 0u);
}

TEST_F(DrawBufferRoutingFixture, AddMeshInstance_IncrementsVersion)
{
    Moonlit::Material mat(std::string(MATERIAL_TEST_SHADERS_DIR) + "/TestMaterial_NoTextures.slang");
    auto* buf = m_renderer.GetOrCreateDrawBuffer(&mat);
    EXPECT_EQ(buf->GetVersion(), 0u);

    auto meshHandle = CreateTestCubeMesh();
    buf->AddMeshInstance(meshHandle, {}, glm::mat4(1.0f));
    EXPECT_EQ(buf->GetVersion(), 1u);
}
