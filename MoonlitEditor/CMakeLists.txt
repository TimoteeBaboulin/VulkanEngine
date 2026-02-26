FILE(GLOB_RECURSE EditorSource CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/Sources/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Headers/*.h
)

ADD_EXECUTABLE(MoonlitEditor ${EditorSource})

SET_TARGET_PROPERTIES(MoonlitEditor PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${RUNTIME_DIR}"
)

TARGET_LINK_LIBRARIES(MoonlitEditor PRIVATE
        MoonlitEngine
        DefaultPlugin
        Qt6::Core Qt6::Widgets Qt6::Gui
        ads::qtadvanceddocking-qt6
)

TARGET_INCLUDE_DIRECTORIES(MoonlitEditor PRIVATE
        Sources
)

ADD_CUSTOM_COMMAND(TARGET MoonlitEditor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:ads::qtadvanceddocking-qt6>
        $<TARGET_FILE_DIR:MoonlitEditor>
)

if(NOT DEFINED MOONLIT_RESOURCES_DIR)
    SET(MOONLIT_RESOURCES_DIR "${CMAKE_SOURCE_DIR}/Resources" CACHE PATH "Moonlit project resources directory")
endif()

ADD_CUSTOM_COMMAND(TARGET MoonlitEditor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${MOONLIT_RESOURCES_DIR}"
        "$<TARGET_FILE_DIR:MoonlitEditor>/Resources"
)

ADD_CUSTOM_COMMAND(TARGET MoonlitEditor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:DefaultPlugin>
    $<TARGET_FILE_DIR:MoonlitEditor>/Modules/$<TARGET_FILE_NAME:DefaultPlugin>
)