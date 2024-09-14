cmake_minimum_required(VERSION 3.28)
include(FetchContent)
project(Insight)

set(CMAKE_CXX_STANDARD 20)

add_library(Insight SHARED src/Insight/Insight.h
        src/Insight/Application.cpp
        src/Insight/Application.cpp
        src/Insight/Application.h
        src/Insight/Assert.h
        src/Insight/Controller.cpp
        src/Insight/Controller.h
        src/Insight/Fs.cpp
        src/Insight/Fs.h
        src/Insight/Insight.h
        src/Insight/Loaders/TextureLoader.cpp
        src/Insight/Loaders/TextureLoader.h
        src/Insight/Loaders/AssetLoader.cpp
        src/Insight/Loaders/AssetLoader.h
        src/Insight/Log.cpp
        src/Insight/Log.h
        src/Insight/Math.h
        src/Insight/Time.cpp
        src/Insight/Time.h
        src/Insight/Events/Event.cpp
        src/Insight/Assets/Texture.cpp
        src/Insight/Assets/Texture.h
        src/Insight/Events/Event.h
        src/Insight/Renderer/Command.cpp
        src/Insight/Renderer/Command.h
        src/Insight/Renderer/CommandBuffer.cpp
        src/Insight/Renderer/CommandBuffer.h
        src/Insight/Renderer/CommandPool.cpp
        src/Insight/Renderer/CommandPool.h
        src/Insight/Renderer/Context.cpp
        src/Insight/Renderer/Context.h
        src/Insight/Renderer/DebugMarkers.cpp
        src/Insight/Renderer/DebugMarkers.h
        src/Insight/Renderer/DescriptorPool.cpp
        src/Insight/Renderer/DescriptorPool.h
        src/Insight/Renderer/DescriptorSet.cpp
        src/Insight/Renderer/DescriptorSet.h
        src/Insight/Renderer/DescriptorSetLayout.cpp
        src/Insight/Renderer/DescriptorSetLayout.h
        src/Insight/Renderer/DynamicGraphicsPipeline.cpp
        src/Insight/Renderer/DynamicGraphicsPipeline.h
        src/Insight/Renderer/Fence.cpp
        src/Insight/Renderer/Fence.h
        src/Insight/Renderer/FinalCompositorPass.cpp
        src/Insight/Renderer/FinalCompositorPass.h
        src/Insight/Renderer/Framebuffer.cpp
        src/Insight/Renderer/Framebuffer.h
        src/Insight/Renderer/GraphicsPipeline.cpp
        src/Insight/Renderer/GraphicsPipeline.h
        src/Insight/Renderer/Image.cpp
        src/Insight/Renderer/Image.h
        src/Insight/Renderer/ImageBarrier.h
        src/Insight/Renderer/ImageView.cpp
        src/Insight/Renderer/ImageView.h
        src/Insight/Renderer/PipelineLayout.cpp
        src/Insight/Renderer/PipelineLayout.h
        src/Insight/Renderer/RenderPass.cpp
        src/Insight/Renderer/RenderPass.h
        src/Insight/Renderer/Sampler.cpp
        src/Insight/Renderer/Sampler.h
        src/Insight/Renderer/SceneRenderer.cpp
        src/Insight/Renderer/SceneRenderer.h
        src/Insight/Renderer/Semaphore.cpp
        src/Insight/Renderer/Semaphore.h
        src/Insight/Renderer/Shader.cpp
        src/Insight/Renderer/Shader.h
        src/Insight/Renderer/SwapChain.cpp
        src/Insight/Renderer/SwapChain.h
        src/Insight/Renderer/SwapChainSupport.h
        src/Insight/Renderer/SynchronizationFrame.cpp
        src/Insight/Renderer/SynchronizationFrame.h
        src/Insight/Renderer/Utils.cpp
        src/Insight/Renderer/Utils.h
        src/Insight/Renderer/Vertex3D.h
        src/Insight/Renderer/VertexBufferLayout.cpp
        src/Insight/Renderer/VertexBufferLayout.h
        src/Insight/Renderer/Vulkan.h
        src/Insight/Renderer/VulkanMemoryAllocator.h
        src/Insight/Ui/Context.cpp
        src/Insight/Ui/Context.h
        src/Insight/Ui/ImGui.cpp
        src/Insight/Ui/ImGui.h
        src/Insight/Windows/Window.cpp
        src/Insight/Windows/Window.h
        src/Insight/Windows/WindowEvents.h
        src/Insight/Ui/Panel.cpp
        src/Insight/Ui/Panel.h
        src/Insight/UUID.cpp
        src/Insight/UUID.h
        src/Insight/Library.cpp
        src/Insight/Library.h
        src/Insight/AssetDescriptor.cpp
        src/Insight/AssetDescriptor.h
        src/Insight/Assets/Asset.cpp
        src/Insight/Assets/Asset.h
        src/Insight/stb_image.cpp
        src/Insight/Ui/Widgets.h
        src/Insight/Ui/Widgets.cpp
        src/Insight/Renderer/Buffer.cpp
        src/Insight/Renderer/Buffer.h
        src/Insight/Ui/ImNodes.cpp
        src/Insight/Ui/ImNodes.h
        src/Insight/Ui/Utils.h
        src/Insight/Ui/FontAwesome6.h
        src/Insight/Scene.cpp
        src/Insight/Scene.h
        src/Insight/Entity.cpp
        src/Insight/Entity.h
        src/Insight/Assets/Material.cpp
        src/Insight/Assets/Material.h
        src/Insight/Assets/StaticMesh.cpp
        src/Insight/Assets/StaticMesh.h
        src/Insight/Components/UuidComponent.h
        src/Insight/Components/NameComponent.h
        src/Insight/Components/StaticMeshComponent.h
        src/Insight/Components/PointLightComponent.h
        src/Insight/Components/CameraComponent.h
        src/Insight/Components/TransformComponent.h
        src/Insight/Geometry/Geometry.cpp
        src/Insight/Geometry/Geometry.h
        src/Insight/Geometry/BoxGeometry.cpp
        src/Insight/Geometry/BoxGeometry.h
        src/Insight/RuntimeLibrary.cpp
        src/Insight/AssetType.cpp
        src/Insight/AssetType.h
        src/Insight/Loaders/MaterialLoader.cpp
        src/Insight/Loaders/MaterialLoader.h
        src/Insight/stb_image_write.cpp
        src/Insight/Base64.cpp
        src/Insight/Base64.h
        src/Insight/Base64.h
        src/Insight/Geometry/SphereGeometry.h
        src/Insight/Geometry/SphereGeometry.cpp
        src/Insight/Geometry/PlaneGeometry.cpp
        src/Insight/Geometry/PlaneGeometry.h
        src/Insight/Components/DirectionalLightComponent.h
        src/Insight/Assets/Prefab.cpp
        src/Insight/Assets/Prefab.h
        src/Insight/Components/PrefabComponent.h
        src/Insight/Components/HierarchyComponent.h
        src/Insight/Loaders/StaticMeshLoader.cpp
        src/Insight/Loaders/StaticMeshLoader.h
        src/Insight/Loaders/StaticMeshLoader.h
        src/Insight/ImageUtils.cpp
        src/Insight/ImageUtils.h
        src/Insight/tinyexr.cpp
        src/Insight/Components/EnvironmentComponent.h
        src/Insight/Renderer/EquirectangularToCubemapRenderer.cpp
        src/Insight/Renderer/EquirectangularToCubemapRenderer.h
        src/Insight/Environment.cpp
        src/Insight/Environment.h
        src/Insight/Renderer/IrradianceRenderer.cpp
        src/Insight/Renderer/IrradianceRenderer.h
        src/Insight/Renderer/DefaultResources.cpp
        src/Insight/Renderer/DefaultResources.h
        src/Insight/Renderer/Passes/GBufferPass.cpp
        src/Insight/Renderer/Passes/GBufferPass.h
        src/Insight/Renderer/Passes/LightingPass.cpp
        src/Insight/Renderer/Passes/LightingPass.h
        src/Insight/Renderer/Passes/OutputPass.cpp
        src/Insight/Renderer/Passes/OutputPass.h
        src/Insight/Renderer/PrefilteredEnvironmentRenderer.cpp
        src/Insight/Renderer/PrefilteredEnvironmentRenderer.h
        src/Insight/Renderer/Passes/BloomPass.h
        src/Insight/Renderer/Passes/BloomPass.cpp
)

find_package(Assimp REQUIRED)
target_include_directories(Insight PUBLIC ${ASSIMP_INCLUDE_DIRS})
target_link_libraries(Insight ${ASSIMP_LIBRARIES})

find_package(Vulkan REQUIRED)

target_include_directories(Insight PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(Insight Vulkan::Vulkan)

find_package(glfw3 3.3 REQUIRED)

target_include_directories(Insight PUBLIC ${GLFW3_INCLUDE_DIR})
target_link_libraries(Insight glfw)

find_package(glm REQUIRED)

target_include_directories(Insight PUBLIC ${GLM_INCLUDE_DIRS})
target_link_libraries(Insight ${GLM_LIBRARIES})

find_package(spdlog REQUIRED)
target_link_libraries(Insight spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)

find_package(nlohmann_json REQUIRED)

target_include_directories(Insight PUBLIC ${nlohmann_json_INCLUDE_DIRS})
target_link_libraries(Insight nlohmann_json::nlohmann_json)

add_subdirectory(vendor/entt vendor/entt/cmake-build-debug)

target_include_directories(Insight PUBLIC ${EnTT_INCLUDE_DIRS})
target_link_libraries(Insight EnTT::EnTT)

FetchContent_Declare(imgui_external
        URL https://github.com/ocornut/imgui/archive/refs/tags/v1.90.9-docking.tar.gz
        EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(imgui_external)

add_library(imgui
        ${imgui_external_SOURCE_DIR}/imgui.cpp
        ${imgui_external_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_external_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_external_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_external_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
        ${imgui_external_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
)

target_include_directories(Insight PUBLIC ${imgui_external_SOURCE_DIR})


function(add_shaders TARGET_NAME)
    set(SHADER_SOURCE_FILES ${ARGN}) # the rest of arguments to this function will be assigned as shader source files

    # Validate that source files have been passed
    list(LENGTH SHADER_SOURCE_FILES FILE_COUNT)
    if(FILE_COUNT EQUAL 0)
        message(FATAL_ERROR "Cannot create a shaders target without any source files")
    endif()

    set(SHADER_COMMANDS)
    set(SHADER_PRODUCTS)

    foreach(SHADER_SOURCE IN LISTS SHADER_SOURCE_FILES)
        cmake_path(ABSOLUTE_PATH SHADER_SOURCE NORMALIZE)
        cmake_path(GET SHADER_SOURCE FILENAME SHADER_NAME)

        # Build command
        list(APPEND SHADER_COMMAND COMMAND)
        list(APPEND SHADER_COMMAND Vulkan::glslc)
        list(APPEND SHADER_COMMAND "${SHADER_SOURCE}")
        list(APPEND SHADER_COMMAND "-o")
        list(APPEND SHADER_COMMAND "${CMAKE_CURRENT_BINARY_DIR}/Engine/Shaders/${SHADER_NAME}.spv")

        # Add product
        list(APPEND SHADER_PRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/Engine/Shaders/${SHADER_NAME}.spv")

    endforeach()

    add_custom_target(${TARGET_NAME} ALL
            COMMENT "Compiling Shaders [${TARGET_NAME}] ${SHADER_SOURCE_FILES}"
            SOURCES ${SHADER_SOURCE_FILES}
            BYPRODUCTS ${SHADER_PRODUCTS}
            COMMAND ${SHADER_COMMAND}
    )
endfunction()

set(SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/Insight/Shaders)
file(GLOB SHADERS ${SHADER_DIR}/*.frag ${SHADER_DIR}/*.vert)
add_shaders(InsightShaders ${SHADERS})

add_dependencies(Insight InsightShaders)

add_custom_target(InsightCopyShaderIncludes ALL
        DEPENDS InsightShaders
        COMMENT "Copying src/Insight/Shaders/includes to $<TARGET_FILE_DIR:Insight>/Engine/Shaders/includes"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Insight/Shaders/includes
        $<TARGET_FILE_DIR:Insight>/Engine/Shaders/includes
)

add_dependencies(Insight InsightCopyShaderIncludes)

target_include_directories(Insight PUBLIC vendor/stb)
target_include_directories(Insight PUBLIC vendor/tinyexr)
target_include_directories(Insight PUBLIC vendor/imnodes)
target_include_directories(Insight PUBLIC vendor/vma)

add_subdirectory(vendor/base64)
target_include_directories(Insight PUBLIC vendor/base64/include)

target_include_directories(Insight PUBLIC vendor/bytesize)