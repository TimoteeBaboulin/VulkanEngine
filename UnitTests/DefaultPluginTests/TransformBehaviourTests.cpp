#include <gtest/gtest.h>

#include "Engine/Component/GameObject.h"
#include "Engine/Scene/Scene.h"
#include "Behaviours/TransformBehaviour.h"

namespace
{
    class TransformBehaviourFixture : public ::testing::Test
    {
    protected:
        Moonlit::MoonlitEngine* m_engine = nullptr;
        Moonlit::Scene m_scene;
        Moonlit::GameObject* m_gameObject = nullptr;
        TransformBehaviour* m_transform = nullptr;

        void SetUp() override
        {
            m_engine = Moonlit::MoonlitEngine::CreateHeadless();
            m_gameObject = new Moonlit::GameObject(Moonlit::GameObjectId{}, m_scene);
            m_transform = new TransformBehaviour(m_gameObject);
        }

        void TearDown() override
        {
            delete m_gameObject;
            delete m_engine;
        }
    };
}

TEST_F(TransformBehaviourFixture, GetParameters_ReturnsThreeEntries)
{
    ASSERT_EQ((int)m_transform->GetParameters().size(), 3);
}

TEST_F(TransformBehaviourFixture, GetParameters_CorrectNames)
{
    auto params = m_transform->GetParameters();
    EXPECT_EQ(params[0]->Name(), "Position");
    EXPECT_EQ(params[1]->Name(), "Scale");
    EXPECT_EQ(params[2]->Name(), "Rotation");
}

TEST_F(TransformBehaviourFixture, DefaultPosition_IsOrigin)
{
    glm::vec3 pos = m_transform->GetPosition();
    EXPECT_FLOAT_EQ(pos.x, 0.0f);
    EXPECT_FLOAT_EQ(pos.y, 0.0f);
    EXPECT_FLOAT_EQ(pos.z, 0.0f);
}

TEST_F(TransformBehaviourFixture, SetPosition_UpdatesGetPosition)
{
    m_transform->SetPosition(glm::vec3(4.0f, 5.0f, 6.0f));
    glm::vec3 pos = m_transform->GetPosition();
    EXPECT_FLOAT_EQ(pos.x, 4.0f);
    EXPECT_FLOAT_EQ(pos.y, 5.0f);
    EXPECT_FLOAT_EQ(pos.z, 6.0f);
}

TEST_F(TransformBehaviourFixture, OnTransformChanged_FiresOnSetPosition)
{
    bool fired = false;
    auto handle = m_transform->OnTransformChanged += [&]() { fired = true; };
    m_transform->SetPosition(glm::vec3(1.0f, 0.0f, 0.0f));
    EXPECT_TRUE(fired);
}

TEST_F(TransformBehaviourFixture, GetModelMat_ReflectsTranslation)
{
    m_transform->SetPosition(glm::vec3(5.0f, 3.0f, 7.0f));
    glm::mat4 mat = m_transform->GetModelMat();
    EXPECT_FLOAT_EQ(mat[3][0], 5.0f);
    EXPECT_FLOAT_EQ(mat[3][1], 3.0f);
    EXPECT_FLOAT_EQ(mat[3][2], 7.0f);
}
