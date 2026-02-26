FILE(GLOB_RECURSE SOURCE CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/Sources/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Sources/*.h
)

ADD_LIBRARY(MoonlitEngine SHARED ${SOURCE}
        Sources/Engine/Modules/Module.h
        Sources/Engine/Modules/ModuleManager.h
        Sources/Engine/Modules/ModuleManager.cpp
        Sources/Engine/Component/BehaviourBank.cpp
        Sources/Engine/Component/BehaviourBank.h
        Sources/Engine/Component/BehaviourManager.cpp
        Sources/Engine/Component/BehaviourManager.h)

SET_TARGET_PROPERTIES(MoonlitEngine PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${RUNTIME_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${RUNTIME_DIR}"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>"
)

TARGET_INCLUDE_DIRECTORIES(MoonlitEngine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Sources
)

TARGET_LINK_LIBRARIES(MoonlitEngine PRIVATE
        Vulkan::Vulkan
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        glm
        stb
        assimp
        slang
        xinput
)

TARGET_COMPILE_DEFINITIONS(MoonlitEngine PRIVATE ENGINE_DLL_EXPORT)

SET_TARGET_PROPERTIES(MoonlitEngine PROPERTIES DEBUG_POSTFIX "")

