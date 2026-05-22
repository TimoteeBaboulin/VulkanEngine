#include <gtest/gtest.h>
#include <Windows.h>

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/DrawBuffer.h"
#include "Engine/Renderer/Material/Material.h"
#include "Engine/Renderer/RendererContext.h"
#include "TestMeshUtils.h"
#include <glm/glm.hpp>

namespace
{
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

    class DrawBufferBatchingFixture : public ::testing::Test
    {
    protected:
        Moonlit::Material* m_mat = nullptr;
        Moonlit::Renderer::DrawBuffer* m_buf = nullptr;

        void SetUp() override
        {
            m_mat = new Moonlit::Material(std::string(MATERIAL_TEST_SHADERS_DIR) + "/TestMaterial_NoTextures.slang");
            m_buf = new Moonlit::Renderer::DrawBuffer(m_mat);
        }

        void TearDown() override
        {
            delete m_buf;
            delete m_mat;
        }
    };
}

// Routing -------------------------------------------------------------------------

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

TEST_F(DrawBufferRoutingFixture, AddMeshInstance_IncrementsVersion)
{
    Moonlit::Material mat(std::string(MATERIAL_TEST_SHADERS_DIR) + "/TestMaterial_NoTextures.slang");
    auto* buf = m_renderer.GetOrCreateDrawBuffer(&mat);
    EXPECT_EQ(buf->GetVersion(), 0u);

    auto meshHandle = CreateTestCubeMesh();
    buf->AddMeshInstance(meshHandle, {}, glm::mat4(1.0f));
    EXPECT_EQ(buf->GetVersion(), 1u);
}

// Version -------------------------------------------------------------------------

TEST(DrawBufferVersion, StartsAtZero)
{
    Moonlit::Material mat(std::string(MATERIAL_TEST_SHADERS_DIR) + "/TestMaterial_NoTextures.slang");
    Moonlit::Renderer::DrawBuffer buf(&mat);
    EXPECT_EQ(buf.GetVersion(), 0u);
}

// Batching ------------------------------------------------------------------------

TEST_F(DrawBufferBatchingFixture, SameMesh_CreatesOneMeshEntry)
{
    auto mesh = CreateTestCubeMesh();
    m_buf->AddMeshInstance(mesh, {}, glm::mat4(1.0f));
    m_buf->AddMeshInstance(mesh, {}, glm::mat4(1.0f));
    auto entries = m_buf->GetMeshEntries();
    ASSERT_EQ(entries.size(), 1u);
    EXPECT_EQ(entries[0].InstanceCount, 2u);
}

TEST_F(DrawBufferBatchingFixture, TwoDifferentMeshes_CreatesTwoMeshEntries)
{
    m_buf->AddMeshInstance(CreateTestCubeMesh(), {}, glm::mat4(1.0f));
    m_buf->AddMeshInstance(CreateTestTriangleMesh(), {}, glm::mat4(1.0f));
    EXPECT_EQ(m_buf->GetMeshEntries().size(), 2u);
}

TEST_F(DrawBufferBatchingFixture, RemoveOneInstance_DecrementsCount)
{
    auto mesh = CreateTestCubeMesh();
    uint32_t id = m_buf->AddMeshInstance(mesh, {}, glm::mat4(1.0f));
    m_buf->AddMeshInstance(mesh, {}, glm::mat4(1.0f));
    m_buf->RemoveMeshInstance(id);
    auto entries = m_buf->GetMeshEntries();
    ASSERT_EQ(entries.size(), 1u);
    EXPECT_EQ(entries[0].InstanceCount, 1u);
}

TEST_F(DrawBufferBatchingFixture, RemoveLastInstance_RemovesMeshEntry)
{
    auto mesh = CreateTestCubeMesh();
    uint32_t id = m_buf->AddMeshInstance(mesh, {}, glm::mat4(1.0f));
    m_buf->RemoveMeshInstance(id);
    EXPECT_EQ(m_buf->GetMeshEntries().size(), 0u);
}

TEST_F(DrawBufferBatchingFixture, VertexCount_SameMeshTwice_NotDuplicated)
{
    auto mesh = CreateTestCubeMesh();
    m_buf->AddMeshInstance(mesh, {}, glm::mat4(1.0f));
    m_buf->AddMeshInstance(mesh, {}, glm::mat4(1.0f));
    EXPECT_EQ(m_buf->GetVertexCount(), 8u);
}

TEST_F(DrawBufferBatchingFixture, VertexCount_TwoDifferentMeshes)
{
    m_buf->AddMeshInstance(CreateTestCubeMesh(), {}, glm::mat4(1.0f));
    m_buf->AddMeshInstance(CreateTestTriangleMesh(), {}, glm::mat4(1.0f));
    EXPECT_EQ(m_buf->GetVertexCount(), 11u);
}
