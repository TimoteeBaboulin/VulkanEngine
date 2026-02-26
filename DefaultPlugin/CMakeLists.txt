FILE(GLOB_RECURSE DefaultPluginSources CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/Sources/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Sources/*.h
)

ADD_LIBRARY(DefaultPlugin SHARED ${DefaultPluginSources})

SET_TARGET_PROPERTIES(DefaultPlugin PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${RUNTIME_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${RUNTIME_DIR}"       # for .so/.dylib, safe on Windows too
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>"  # .lib
        DEBUG_POSTFIX ""
)

TARGET_INCLUDE_DIRECTORIES(DefaultPlugin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Sources)
TARGET_LINK_LIBRARIES(DefaultPlugin PRIVATE MoonlitEngine Qt6::Core Qt6::Widgets Qt6::Gui)