FILE(GLOB_RECURSE SOURCE CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/Sources/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Sources/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Sources/*.ipp
)



ADD_LIBRARY(MoonlitEngine SHARED)

TARGET_SOURCES(
        MoonlitEngine PRIVATE
        Sources/Camera.cpp
        Sources/main.cpp
        Sources/Debug/Logger.cpp
        Sources/Debug/Profiler.cpp
        Sources/Debug/Exceptions/PluginLoadException.cpp
        Sources/Engine/Component/BehaviourBank.cpp
        Sources/Engine/Component/BehaviourRegistry.cpp
        Sources/Engine/Component/GameObject.cpp
        Sources/Engine/Component/ObjectBehaviour.cpp
        Sources/Engine/Component/ParameterBase.ipp
        Sources/Engine/Inputs/InputManager.cpp
        Sources/Engine/Inputs/PlatformInputAbstraction.cpp
        Sources/Engine/Inputs/QtEventReader.cpp
        Sources/Engine/Inputs/WindowsInputAbstraction.cpp
        Sources/Engine/Modules/ModuleManager.cpp
        Sources/Engine/Modules/CMakeGenerator.cpp
        Sources/Engine/Renderer/Material/Material.cpp
        Sources/Engine/Renderer/Material/MaterialInstance.cpp
        Sources/Engine/Renderer/BufferDeviceLink.cpp
        Sources/Engine/Renderer/DrawBuffer.cpp
        Sources/Engine/Renderer/Renderer.cpp
        Sources/Engine/Renderer/RendererContext.cpp
        Sources/Engine/Renderer/RendererDeviceManager.cpp
        Sources/Engine/Renderer/RenderTarget.cpp
        Sources/Engine/ResourceManagement/Helpers/FileHelper.cpp
        Sources/Engine/ResourceManagement/Helpers/ImageHelper.cpp
        Sources/Engine/ResourceManagement/Banks/MeshBank.cpp
        Sources/Engine/ResourceManagement/ResourceManager.cpp
        Sources/Engine/ResourceManagement/Banks/TextureBank.cpp
        Sources/Engine/Scene/Scene.cpp
        Sources/Engine/Tasks/Task.cpp
        Sources/Engine/Tasks/WorkerManager.cpp
        Sources/Engine/MoonlitEngine.cpp
)

TARGET_SOURCES(
        MoonlitEngine PUBLIC
        Sources/common.h
        Sources/GLMExport.h
        Sources/MoonlitExport.h
        Sources/Debug/Logger.h
        Sources/Debug/Profiler.h
        Sources/Debug/Exceptions/PluginLoadException.h
        Sources/Debug/Exceptions/MoonlitExceptions.h
        Sources/Engine/Component/BehaviourBank.h
        Sources/Engine/Component/BehaviourRegistry.h
        Sources/Engine/Component/GameObject.h
        Sources/Engine/Component/ObjectBehaviour.h
        Sources/Engine/Component/ParameterBase.h
        Sources/Engine/Component/Parameter.h
        Sources/Engine/Entity/Entity.h
        Sources/Engine/Events/Event.h
        Sources/Engine/Events/EventUtility.h
        Sources/Engine/Inputs/InputManager.h
        Sources/Engine/Inputs/Inputs.h
        Sources/Engine/Inputs/PlatformInputAbstraction.h
        Sources/Engine/Inputs/QtEventReader.h
        Sources/Engine/Inputs/WindowsInputAbstraction.h
        Sources/Engine/Modules/Module.h
        Sources/Engine/Modules/ModuleManager.h
        Sources/Engine/Modules/CMakeGenerator.h
        Sources/Engine/Renderer/Helpers/VertexInputHelper.h
        Sources/Engine/Renderer/Helpers/VulkanHelperFunctions.h
        Sources/Engine/Renderer/Material/Material.h
        Sources/Engine/Renderer/Material/MaterialInstance.h
        Sources/Engine/Renderer/BufferDeviceLink.h
        Sources/Engine/Renderer/Camera.h
        Sources/Engine/Renderer/CustomVulkanImport.h
        Sources/Engine/Renderer/CustomVulkanStructs.h
        Sources/Engine/Renderer/DrawBuffer.h
        Sources/Engine/Renderer/Renderer.h
        Sources/Engine/Renderer/RendererContext.h
        Sources/Engine/Renderer/RendererDeviceManager.h
        Sources/Engine/Renderer/RenderTarget.h
        Sources/Engine/ResourceManagement/Helpers/FileHelper.h
        Sources/Engine/ResourceManagement/Helpers/ImageHelper.h
        Sources/Engine/ResourceManagement/Resources/Image.h
        Sources/Engine/ResourceManagement/Resources/Mesh.h
        Sources/Engine/ResourceManagement/Banks/MeshBank.h
        Sources/Engine/ResourceManagement/ResourceBank.h
        Sources/Engine/ResourceManagement/ResourceBank.ipp
        Sources/Engine/ResourceManagement/ResourceHandle.h
        Sources/Engine/ResourceManagement/ResourceHandle.ipp
        Sources/Engine/ResourceManagement/ResourceManager.h
        Sources/Engine/ResourceManagement/ResourceManager.ipp
        Sources/Engine/ResourceManagement/Banks/TextureBank.h
        Sources/Engine/ResourceManagement/Resources/TextureData.h
        Sources/Engine/Scene/Scene.h
        Sources/Engine/Tasks/Task.h
        Sources/Engine/Tasks/WorkerManager.h
        Sources/Engine/MoonlitEngine.h
)



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
        stb
        assimp
        slang
        xinput
        nlohmann_json::nlohmann_json
)

TARGET_LINK_LIBRARIES(MoonlitEngine PUBLIC
        glm
)

TARGET_COMPILE_DEFINITIONS(MoonlitEngine PRIVATE ENGINE_DLL_EXPORT)

SET_TARGET_PROPERTIES(MoonlitEngine PROPERTIES DEBUG_POSTFIX "")

ADD_CUSTOM_COMMAND(TARGET MoonlitEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:MoonlitEngine>
        $<TARGET_FILE_DIR:EngineTests>
)