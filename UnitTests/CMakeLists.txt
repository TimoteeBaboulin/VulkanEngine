FETCHCONTENT_DECLARE(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FETCHCONTENT_MAKEAVAILABLE(googletest)

ADD_EXECUTABLE(EngineTests test_main.cpp
        "TaskSystemTests/TaskSystemTestMain.cpp")
TARGET_LINK_LIBRARIES(EngineTests MoonlitEngine gtest_main)

ADD_CUSTOM_COMMAND(TARGET EngineTests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:EngineTests> $<TARGET_FILE_DIR:EngineTests>
        COMMAND_EXPAND_LISTS
)

INCLUDE(googletest)
GTEST_DISCOVER_TESTS(EngineTests)