FETCHCONTENT_DECLARE(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FETCHCONTENT_MAKEAVAILABLE(googletest)

ADD_EXECUTABLE(EngineTests test_main.cpp
        "TaskSystemTests/TaskSystemTestMain.cpp"
        "MaterialTests/MaterialTests.cpp")
TARGET_LINK_LIBRARIES(EngineTests MoonlitEngine Vulkan::Vulkan gtest_main)
TARGET_COMPILE_DEFINITIONS(EngineTests PRIVATE
        "MATERIAL_TEST_SHADERS_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/MaterialTests/Shaders\""
)

ADD_CUSTOM_COMMAND(TARGET EngineTests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:EngineTests> $<TARGET_FILE_DIR:EngineTests>
        COMMAND_EXPAND_LISTS
)

INCLUDE(googletest)
GTEST_DISCOVER_TESTS(EngineTests)

ADD_EXECUTABLE(DefaultPluginTests
        "DefaultPluginTests/TransformBehaviourTests.cpp")
TARGET_LINK_LIBRARIES(DefaultPluginTests DefaultPlugin MoonlitEngine Vulkan::Vulkan gtest_main)
ADD_CUSTOM_COMMAND(TARGET DefaultPluginTests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:DefaultPluginTests> $<TARGET_FILE_DIR:DefaultPluginTests>
        COMMAND_EXPAND_LISTS
)
GTEST_DISCOVER_TESTS(DefaultPluginTests)