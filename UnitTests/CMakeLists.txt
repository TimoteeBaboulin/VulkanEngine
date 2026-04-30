FETCHCONTENT_DECLARE(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FETCHCONTENT_MAKEAVAILABLE(googletest)

ADD_EXECUTABLE(EngineTests test_main.cpp
        "TaskSystemTests/TaskSystemTestMain.cpp")
TARGET_LINK_LIBRARIES(EngineTests MoonlitEngine gtest_main)
ADD_TEST(NAME EngineTests COMMAND EngineTests)