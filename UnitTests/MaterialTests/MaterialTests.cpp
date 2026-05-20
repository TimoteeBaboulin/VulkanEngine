#include <gtest/gtest.h>

#include "Engine/Renderer/Material/Material.h"

namespace Moonlit
{
    TEST(MaterialTests, TextureCount_SingleSlot)
    {
        Material mat(std::string(MATERIAL_TEST_SHADERS_DIR) + "/TestMaterial_SingleTexture.slang");
        EXPECT_EQ(mat.GetTextureCount(), 1);
    }

    TEST(MaterialTests, TextureCount_ThreeSlots)
    {
        Material mat(std::string(MATERIAL_TEST_SHADERS_DIR) + "/TestMaterial_ThreeTextures.slang");
        EXPECT_EQ(mat.GetTextureCount(), 3);
    }

    TEST(MaterialTests, TextureCount_NoSlots)
    {
        Material mat(std::string(MATERIAL_TEST_SHADERS_DIR) + "/TestMaterial_NoTextures.slang");
        EXPECT_EQ(mat.GetTextureCount(), 0);
    }

    TEST(MaterialTests, GlobalResources_NamesMatchFields)
    {
        Material mat(std::string(MATERIAL_TEST_SHADERS_DIR) + "/TestMaterial_ThreeTextures.slang");
        const auto& resources = mat.GetShaderData().GlobalResources;

        ASSERT_EQ((int)resources.size(), 3);
        EXPECT_EQ(resources[0].Name, "albedo");
        EXPECT_EQ(resources[1].Name, "normal");
        EXPECT_EQ(resources[2].Name, "roughness");
    }
}