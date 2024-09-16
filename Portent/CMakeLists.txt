cmake_minimum_required(VERSION 3.28)
project(Portent)

set(CMAKE_CXX_STANDARD 20)

add_executable(Portent src/main.cpp
        src/EditorApplication.cpp
        src/EditorApplication.h
        src/Editor.cpp
        src/Editor.h
        src/DockingHost.cpp
        src/DockingHost.h
        src/Project.cpp
        src/Project.h
        src/Project.cpp
        src/Portent.h
        src/Editors/TextureEditor/TextureEditor.h
        src/Editors/TextureEditor/TextureEditor.cpp
        src/Editors/EditorPanel.cpp
        src/Editors/EditorPanel.h
        src/DemoWindow.cpp
        src/DemoWindow.h
        src/Controls/FileBrowser.cpp
        src/Controls/FileBrowser.h
        src/ContentBrowser.h
        src/ContentBrowser.cpp
        src/Editors/TextureEditor/TextureEditorProperties.cpp
        src/Editors/TextureEditor/TextureEditorProperties.h
        src/Editors/MaterialEditor/MaterialEditor.cpp
        src/Editors/MaterialEditor/MaterialEditor.h
        src/Editors/MaterialEditor/MaterialEditorProperties.cpp
        src/Editors/MaterialEditor/MaterialEditorProperties.h
        src/NodeGraph/Graph.cpp
        src/NodeGraph/Graph.h
        src/NodeGraph/Node.cpp
        src/NodeGraph/Node.h
        src/NodeGraph/Attribute.cpp
        src/NodeGraph/Attribute.h
        src/NodeGraph/Value.cpp
        src/NodeGraph/Value.h
        src/Editors/MaterialEditor/Nodes/SurfaceOutputNode.cpp
        src/Editors/MaterialEditor/Nodes/SurfaceOutputNode.h
        src/NodeGraph/GraphTypes.h
        src/NodeGraph/Renderer.cpp
        src/NodeGraph/Renderer.h
        src/Editors/MaterialEditor/Nodes/Texture2DSampleNode.cpp
        src/Editors/MaterialEditor/Nodes/Texture2DSampleNode.h
        src/NodeGraph/TextureRef.cpp
        src/NodeGraph/TextureRef.h
        src/Widgets.cpp
        src/Widgets.h
        src/Editors/MaterialEditor/Nodes/Texture2DNode.cpp
        src/Editors/MaterialEditor/Nodes/Texture2DNode.h
        src/Editors/MaterialEditor/Nodes/TexCoordsNode.cpp
        src/Editors/MaterialEditor/Nodes/TexCoordsNode.h
        src/Editors/MaterialEditor/MaterialEditorPreview.cpp
        src/Editors/MaterialEditor/MaterialEditorPreview.h
        src/Editors/MaterialEditor/Compiler/GlslGraphCompiler.cpp
        src/Editors/MaterialEditor/Compiler/GlslGraphCompiler.h
        src/Editors/MaterialEditor/Nodes/MaterialNode.cpp
        src/Editors/MaterialEditor/Nodes/MaterialNode.h
        src/Editors/MaterialEditor/Nodes/UnpackNormalNode.h
        src/Editors/MaterialEditor/Nodes/UnpackNormalNode.cpp
        src/Editors/MaterialEditor/Nodes/BinaryOpNode.cpp
        src/Editors/MaterialEditor/Nodes/BinaryOpNode.h
        src/Editors/MaterialEditor/Nodes/TimeNode.h
        src/Editors/MaterialEditor/Nodes/TimeNode.cpp
        src/Editors/MaterialEditor/Nodes/AddNode.cpp
        src/Editors/MaterialEditor/Nodes/AddNode.h
        src/Editors/MaterialEditor/Nodes/ModuloNode.h
        src/Editors/MaterialEditor/Nodes/ModuloNode.cpp
        src/Editors/MaterialEditor/Nodes/SubtractNode.h
        src/Editors/MaterialEditor/Nodes/SubtractNode.cpp
        src/Editors/MaterialEditor/Nodes/SinNode.h
        src/Editors/MaterialEditor/Nodes/SinNode.cpp
        src/EditorLibrary.cpp
        src/EditorLibrary.h
        src/Savers/TextureSaver.cpp
        src/Savers/TextureSaver.h
        src/Savers/AssetSaver.h
        src/Savers/AssetSaver.cpp
        src/EditorResources.cpp
        src/EditorResources.h
        src/Thumbnails/ThumbnailManager.cpp
        src/Thumbnails/ThumbnailManager.h
        src/Thumbnails/TextureThumbnailGenerator.cpp
        src/Thumbnails/TextureThumbnailGenerator.h
        src/Thumbnails/ThumbnailGenerator.cpp
        src/Thumbnails/ThumbnailGenerator.h
        src/NodeGraph/NodeFactory.cpp
        src/NodeGraph/NodeFactory.h
        src/Editors/MaterialEditor/Nodes/MaterialNodeFactory.cpp
        src/Editors/MaterialEditor/Nodes/MaterialNodeFactory.h
        src/Thumbnails/MaterialThumbnailGenerator.h
        src/Thumbnails/MaterialThumbnailGenerator.cpp
        src/Editors/MaterialEditor/Nodes/DotProductNode.h
        src/Editors/MaterialEditor/Nodes/DotProductNode.cpp
        src/Editors/MaterialEditor/Nodes/SquareRootNode.h
        src/Editors/MaterialEditor/Nodes/SquareRootNode.cpp
        src/Editors/MaterialEditor/Nodes/VectorToComponentsNode.cpp
        src/Editors/MaterialEditor/Nodes/ComponentsToVectorNode.cpp
        src/Editors/MaterialEditor/Nodes/ComponentsToVectorNode.h
        src/Editors/MaterialEditor/Nodes/MultiplyNode.h
        src/Editors/MaterialEditor/Nodes/MultiplyNode.cpp
        src/Importers/AssetImporter.cpp
        src/Importers/AssetImporter.h
        src/Importers/TextureImporter.cpp
        src/Importers/TextureImporter.h
        src/Wizards/AssetImportWizard.cpp
        src/Wizards/AssetImportWizard.h
        src/Wizards/StaticMeshImportWizard.cpp
        src/Wizards/StaticMeshImportWizard.h
        src/Editors/PrefabEditor/PrefabEditor.cpp
        src/Editors/PrefabEditor/PrefabEditor.h
        src/Editors/PrefabEditor/SceneExplorer.cpp
        src/Editors/PrefabEditor/SceneExplorer.h
        src/Editors/PrefabEditor/ObjectProperties.cpp
        src/Editors/PrefabEditor/ObjectProperties.h
        src/OrbitCameraController.cpp
        src/OrbitCameraController.h
        src/Editors/MaterialEditor/Nodes/ParallaxOcclusionMappingNode.cpp
        src/Editors/MaterialEditor/Nodes/ParallaxOcclusionMappingNode.h)

target_compile_definitions(Portent PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")

add_subdirectory(../Insight ../Insight/cmake-build-debug)
target_link_libraries(Portent PUBLIC Insight)



add_custom_target(PortentInsightCopyShaders ALL
        COMMENT "Copying $<TARGET_FILE_DIR:Insight>/Engine/Shaders to $<TARGET_FILE_DIR:Portent>/Engine/Shaders"
        DEPENDS Insight
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        $<TARGET_FILE_DIR:Insight>/Engine/Shaders
        $<TARGET_FILE_DIR:Portent>/Engine/Shaders
)

add_custom_target(PortentInsightCopyFonts ALL
        COMMENT "Copying ../Insight/Engine/Fonts to $<TARGET_FILE_DIR:Portent>/Engine/Fonts"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ../../Insight/Engine/Fonts
        $<TARGET_FILE_DIR:Portent>/Engine/Fonts
)

add_custom_target(PortentInsightCopyTextures ALL
        COMMENT "Copying ../Insight/Engine/Textures to $<TARGET_FILE_DIR:Portent>/Engine/Textures"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ../../Insight/Engine/Textures
        $<TARGET_FILE_DIR:Portent>/Engine/Textures
)

add_custom_target(PortentInsightCopyShaderIncludes ALL
        COMMENT "Copying $<TARGET_FILE_DIR:Insight>/Engine/Shaders/includes to $<TARGET_FILE_DIR:Portent>/Engine/Shaders/includes"
        DEPENDS Insight
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        $<TARGET_FILE_DIR:Insight>/Engine/Shaders/includes
        $<TARGET_FILE_DIR:Portent>/Engine/Shaders/includes
)

add_custom_target(PortentCopyAssetIcons ALL
        COMMENT "Copying Editor/AssetIcons to $<TARGET_FILE_DIR:Portent>/Editor/AssetIcons"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/Editor/AssetIcons
        $<TARGET_FILE_DIR:Portent>/Editor/AssetIcons
)

add_custom_target(PortentCopyTextures ALL
        COMMENT "Copying Editor/Textures to $<TARGET_FILE_DIR:Portent>/Editor/Textures"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/Editor/Textures
        $<TARGET_FILE_DIR:Portent>/Editor/Textures
)

add_custom_target(PortentCopyHDRI ALL
        COMMENT "Copying Editor/HDRI to $<TARGET_FILE_DIR:Portent>/Editor/HDRI"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/Editor/HDRI
        $<TARGET_FILE_DIR:Portent>/Editor/HDRI
)

add_dependencies(Portent PortentInsightCopyShaders)
add_dependencies(Portent PortentInsightCopyFonts)
add_dependencies(Portent PortentInsightCopyTextures)
add_dependencies(Portent PortentInsightCopyShaderIncludes)
add_dependencies(Portent PortentCopyAssetIcons)
add_dependencies(Portent PortentCopyTextures)
add_dependencies(Portent PortentCopyHDRI)

target_include_directories(Portent PUBLIC ../Insight/src)

target_include_directories(Portent PUBLIC vendor/subprocess.h)

add_subdirectory(vendor/nativefiledialog-extended)
target_link_libraries(Portent PUBLIC nfd)
target_include_directories(Portent PUBLIC vendor/nativefiledialog-extended/src/include)

find_package(Vulkan REQUIRED)

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
        list(APPEND SHADER_COMMAND "-I")
        list(APPEND SHADER_COMMAND "${CMAKE_CURRENT_BINARY_DIR}/Engine/Shaders")
        list(APPEND SHADER_COMMAND "-o")
        list(APPEND SHADER_COMMAND "${CMAKE_CURRENT_BINARY_DIR}/Editor/Shaders/${SHADER_NAME}.spv")

        # Add product
        list(APPEND SHADER_PRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/Editor/Shaders/${SHADER_NAME}.spv")

    endforeach()

    add_custom_target(${TARGET_NAME} ALL
            COMMENT "Compiling Shaders [${TARGET_NAME}] ${SHADER_SOURCE_FILES}"
            DEPENDS PortentInsightCopyShaderIncludes
            SOURCES ${SHADER_SOURCE_FILES}
            BYPRODUCTS ${SHADER_PRODUCTS}
            COMMAND ${SHADER_COMMAND}
    )
endfunction()

set(SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders)
file(GLOB SHADERS ${SHADER_DIR}/*.frag ${SHADER_DIR}/*.vert)
add_shaders(PortentShaders ${SHADERS})

add_dependencies(Portent PortentShaders)
