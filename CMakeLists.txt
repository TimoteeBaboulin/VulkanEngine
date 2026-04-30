CMAKE_MINIMUM_REQUIRED(VERSION 3.1...4.2.3)

PROJECT(Moonlit VERSION 0.1.0 LANGUAGES CXX)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_VERBOSE_MAKEFILE ON)
SET(CMAKE_PREFIX_PATH "C:/Qt/6.10.2/msvc2022_64")
SET(BUILD_SHARED_LIBS OFF CACHE BOOL "Force static libs for third-party deps" FORCE)
SET(RUNTIME_DIR ${CMAKE_BINARY_DIR}/bin/$<CONFIG>)


INCLUDE(FetchContent)

FIND_PACKAGE(Vulkan REQUIRED)

IF(MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")
ENDIF()
FIND_PACKAGE(Qt6 6.10 REQUIRED COMPONENTS Core Gui Widgets)

# glm
FETCHCONTENT_DECLARE(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG 1.0.1
)

FETCHCONTENT_MAKEAVAILABLE(glm)

#stb
FETCHCONTENT_DECLARE(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
)
FETCHCONTENT_MAKEAVAILABLE(stb)

# assimp
FETCHCONTENT_DECLARE(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp
        GIT_TAG e0b52347c6e52de2827ec957a9ebf00ce3c54f79
)

FETCHCONTENT_MAKEAVAILABLE(assimp)

# slang
FETCHCONTENT_DECLARE(
        slang
        GIT_REPOSITORY https://github.com/shader-slang/slang.git
        GIT_TAG v2025.18.1
)

SET(SLANG_ENABLE_SPIRV_TOOLS_MIMALLOC OFF)

FETCHCONTENT_MAKEAVAILABLE(slang)

# QtADS
FETCHCONTENT_DECLARE(
        ads
        GIT_REPOSITORY https://github.com/githubuser0xFFFF/Qt-Advanced-Docking-System.git
        GIT_SHALLOW FALSE
)

FETCHCONTENT_MAKEAVAILABLE(ads)

FETCHCONTENT_DECLARE(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

FETCHCONTENT_MAKEAVAILABLE(json)

ADD_SUBDIRECTORY(MoonlitEngine)
ADD_SUBDIRECTORY(DefaultPlugin)
ADD_SUBDIRECTORY(MoonlitEditor)

ADD_SUBDIRECTORY("UnitTests")