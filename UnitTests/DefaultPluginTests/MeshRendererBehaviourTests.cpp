#include <gtest/gtest.h>
#include <Windows.h>

#include "Engine/MoonlitEngine.h"
#include "Engine/Component/GameObject.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/DrawBuffer.h"
#include "Engine/Renderer/RendererContext.h"
#include "Engine/ResourceManagement/Banks/MeshBank.h"
#include "Engine/ResourceManagement/Banks/MaterialBank.h"
#include "Engine/ResourceManagement/ResourceManager.h"
#include "Behaviours/MeshRendererBehaviour.h"

namespace
{
    class MeshRendererFixture : public ::testing::Test
    {
    protected:
        Moonlit::MoonlitEngine* m_engine = nullptr;
        Moonlit::Renderer::MoonlitRenderer m_renderer;
        HWND m_hwnd = nullptr;
        Moonlit::Scene m_scene;
        Moonlit::GameObject* m_gameObject = nullptr;
        MeshRendererBehaviour* m_meshRenderer = nullptr;
        bool m_initialized = false;

        void SetUp() override
        {
            m_engine = Moonlit::MoonlitEngine::CreateHeadless();

            WNDCLASSEX wc{};
            wc.cbSize = sizeof(WNDCLASSEX);
            wc.lpfnWndProc = DefWindowProc;
            wc.hInstance = GetModuleHandle(nullptr);
            wc.lpszClassName = "MeshRendererTestWnd";
            RegisterClassEx(&wc);
            m_hwnd = CreateWindowEx(0, "MeshRendererTestWnd", "Test", WS_OVERLAPPEDWINDOW,
                0, 0, 800, 600, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);

            Moonlit::Renderer::ContextInfo info;
            info.name = "MeshRendererTest";
            m_renderer.Init(info, {"VK_KHR_surface", "VK_KHR_win32_surface"});
            m_renderer.AddRenderTarget(m_hwnd, nullptr);
            m_engine->Renderer = &m_renderer;

            auto& meshBank = Moonlit::ResourceManagement::MeshBank::Get();
            if (!meshBank.Exist("Cube"))
            {
                static Moonlit::Vertex verts[8] = {
                    {{-0.5f,-0.5f,-0.5f}}, {{ 0.5f,-0.5f,-0.5f}},
                    {{ 0.5f, 0.5f,-0.5f}}, {{-0.5f, 0.5f,-0.5f}},
                    {{-0.5f,-0.5f, 0.5f}}, {{ 0.5f,-0.5f, 0.5f}},
                    {{ 0.5f, 0.5f, 0.5f}}, {{-0.5f, 0.5f, 0.5f}}
                };
                static uint16_t idx[36] = {0,1,2,2,3,0,4,5,6,6,7,4,0,4,7,7,3,0,1,5,6,6,2,1,3,2,6,6,7,3,0,1,5,5,4,0};
                Moonlit::MeshData cube{};
                cube.vertexCount = 8; cube.vertices = verts; cube.triangleCount = 12; cube.indices = idx;
                meshBank.Insert("Cube", cube);
            }

            auto& matBank = Moonlit::ResourceManagement::MaterialBank::Get();
            if (!matBank.Exist("BaseMaterial"))
                matBank.Insert("BaseMaterial", std::string(MATERIAL_TEST_SHADERS_DIR) + "/TestMaterial_NoTextures.slang");
            if (!matBank.Exist("AltMaterial"))
                matBank.Insert("AltMaterial", std::string(MATERIAL_TEST_SHADERS_DIR) + "/TestMaterial_NoTextures.slang");

            m_gameObject = new Moonlit::GameObject(Moonlit::GameObjectId{}, m_scene);
            m_meshRenderer = new MeshRendererBehaviour(m_gameObject);
        }

        void TearDown() override
        {
            if (m_initialized)
            {
                m_meshRenderer->Dispose();
                m_initialized = false;
            }
            delete m_gameObject;
            m_engine->Renderer = nullptr;
            delete m_engine;
            DestroyWindow(m_hwnd);
            UnregisterClass("MeshRendererTestWnd", GetModuleHandle(nullptr));
        }

        void Init()
        {
            m_meshRenderer->Init();
            m_initialized = true;
        }

        Moonlit::Renderer::DrawBuffer* GetDrawBuffer(const std::string& _name)
        {
            Moonlit::Renderer::MaterialHandle h;
            Moonlit::ResourceManagement::ResourceManager::TryGetResource<Moonlit::Material>(_name, h);
            return m_renderer.GetOrCreateDrawBuffer(&(*h));
        }

        template<class T>
        void SetParam(const std::string& _name, const T& _value)
        {
            for (auto* p : m_meshRenderer->GetParameters())
            {
                if (p->Name() == _name)
                {
                    *dynamic_cast<Parameter<T>*>(p) = _value;
                    m_meshRenderer->ParameterChanged(p);
                    return;
                }
            }
        }
    };
}

TEST_F(MeshRendererFixture, Init_RegistersMeshEntryInDrawBuffer)
{
    Init();
    EXPECT_EQ(GetDrawBuffer("BaseMaterial")->GetMeshEntries().size(), 1u);
}

TEST_F(MeshRendererFixture, Init_VertexCountMatchesCubeMesh)
{
    Init();
    EXPECT_EQ(GetDrawBuffer("BaseMaterial")->GetVertexCount(), 8u);
}

TEST_F(MeshRendererFixture, Dispose_RemovesEntryFromDrawBuffer)
{
    Init();
    m_meshRenderer->Dispose();
    m_initialized = false;
    EXPECT_EQ(GetDrawBuffer("BaseMaterial")->GetMeshEntries().size(), 0u);
}

TEST_F(MeshRendererFixture, MeshChange_UpdatesVertexCount)
{
    Init();

    auto& meshBank = Moonlit::ResourceManagement::MeshBank::Get();
    if (!meshBank.Exist("TestTriangle"))
    {
        static Moonlit::Vertex verts[3] = {
            {{ 0.0f,  0.5f, 0.0f}},
            {{ 0.5f, -0.5f, 0.0f}},
            {{-0.5f, -0.5f, 0.0f}}
        };
        static uint16_t idx[3] = {0, 1, 2};
        Moonlit::MeshData tri{};
        tri.vertexCount = 3; tri.vertices = verts; tri.triangleCount = 1; tri.indices = idx;
        meshBank.Insert("TestTriangle", tri);
    }

    auto triHandle = meshBank.GetResource("TestTriangle");
    SetParam<Moonlit::ResourceManagement::ResourceHandle<Moonlit::MeshData>>("MeshData", triHandle);

    EXPECT_EQ(GetDrawBuffer("BaseMaterial")->GetVertexCount(), 3u);
}

TEST_F(MeshRendererFixture, MaterialChange_MovesEntryToNewDrawBuffer)
{
    Init();

    Moonlit::Renderer::MaterialHandle altHandle;
    Moonlit::ResourceManagement::ResourceManager::TryGetResource<Moonlit::Material>("AltMaterial", altHandle);
    SetParam<Moonlit::Renderer::MaterialHandle>("Material", altHandle);

    EXPECT_EQ(GetDrawBuffer("BaseMaterial")->GetMeshEntries().size(), 0u);
    EXPECT_EQ(GetDrawBuffer("AltMaterial")->GetMeshEntries().size(), 1u);
}
