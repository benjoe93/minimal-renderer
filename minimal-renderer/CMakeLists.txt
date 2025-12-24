cmake_minimum_required(VERSION 3.29)
project(minimal_renderer LANGUAGES CXX C)

# Use C++20 to match your VS project
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Collect source files
file(GLOB MAIN_SOURCES
    "src/*.cpp"
    "src/*.c"
)

set(SCENE_SOURCES
    "src/scenes/00_SceneClearColor.cpp"
    "src/scenes/01_SceneHelloTriagle.cpp"
    "src/scenes/02_SceneHelloTriagleIndexed.cpp"
    "src/scenes/03_SceneTextures.cpp"
    "src/scenes/04_SceneTransformation.cpp"
    "src/scenes/05_SceneCoordinateSystem.cpp"
    "src/scenes/06_SceneCube.cpp"
    "src/scenes/07_SceneCubeMultiple.cpp"
    "src/scenes/08_ScenePhongLight.cpp"
    "src/scenes/09_SceneMaterials.cpp"
    "src/scenes/10_SceneSurfMaps.cpp"
    "src/scenes/11_SceneLightCasters.cpp"
    "src/scenes/12_SceneMultipleLights.cpp"
    "src/scenes/13_SceneModelLoading.cpp"
    "src/scenes/14_SceneDepthTesting.cpp"
    "src/scenes/15_SceneStencilTesting.cpp"
    "src/scenes/16_SceneBlending.cpp"
    "src/scenes/17_SceneFaceCulling.cpp"
    "src/scenes/18_SceneFrameBuffers.cpp"
    "src/scenes/19_SceneRearViewMirror.cpp"
    "src/scenes/20_SceneCubemap.cpp"
    "src/scenes/21_SceneAdvancedGLSL.cpp"
    # "src/scenes/_SceneEmpty.cpp"  # Template - not compiled
)

file(GLOB VENDOR_SOURCES
    "src/vendor/glad/*.c"
    "src/vendor/imgui/*.cpp"
    "src/vendor/stb_image/*.cpp"
)

file(GLOB MAIN_HEADERS
    "src/*.h"
)

set(SCENE_HEADERS
    "src/scenes/Scene.h"
    "src/scenes/00_SceneClearColor.h"
    "src/scenes/01_SceneHelloTriagle.h"
    "src/scenes/02_SceneHelloTriagleIndexed.h"
    "src/scenes/03_SceneTextures.h"
    "src/scenes/04_SceneTransformation.h"
    "src/scenes/05_SceneCoordinateSystem.h"
    "src/scenes/06_SceneCube.h"
    "src/scenes/07_SceneCubeMultiple.h"
    "src/scenes/08_ScenePhongLight.h"
    "src/scenes/09_SceneMaterials.h"
    "src/scenes/10_SceneSurfMaps.h"
    "src/scenes/11_SceneLightCasters.h"
    "src/scenes/12_SceneMultipleLights.h"
    "src/scenes/13_SceneModelLoading.h"
    "src/scenes/14_SceneDepthTesting.h"
    "src/scenes/15_SceneStencilTesting.h"
    "src/scenes/16_SceneBlending.h"
    "src/scenes/17_SceneFaceCulling.h"
    "src/scenes/18_SceneFrameBuffers.h"
    "src/scenes/19_SceneRearViewMirror.h"
    "src/scenes/20_SceneCubemap.h"
    "src/scenes/21_SceneAdvancedGLSL.h"
    # "src/scenes/_SceneEmpty.h"  # Template - not compiled
)

file(GLOB VENDOR_HEADERS
    "src/vendor/imgui/*.h"
    "src/vendor/stb_image/*.h"
)

# Create executable
add_executable(${PROJECT_NAME}
    ${MAIN_SOURCES}
    ${SCENE_SOURCES}
    ${VENDOR_SOURCES}
    ${MAIN_HEADERS}
    ${SCENE_HEADERS}
    ${VENDOR_HEADERS}
)

# Include directories - your manual libraries folder
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/include
)

# Link directories - point to your manual libraries
target_link_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/lib
)

# Link libraries directly (no find_package needed for manual setup)
target_link_libraries(${PROJECT_NAME} PRIVATE
    opengl32        # Windows OpenGL
    glfw3           # GLFW static lib
    assimp-vc143-mt # Assimp (exact name from your VS project)
)

# Platform-specific settings
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        _CRT_SECURE_NO_WARNINGS
    )
    
    # Console subsystem (matches VS project)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE FALSE
    )
endif()

# Compiler settings to match VS project
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W3  # Warning level 3
    )
    
    # Match VS project configurations
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:_DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
    )
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
    )
endif()

# Set output directories to match VS project structure
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/x64/Debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/x64/Release
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/x64/$<CONFIG>
)

# Group source files in IDEs (for Visual Studio)
source_group("Source Files" FILES ${MAIN_SOURCES})
source_group("Source Files\\scenes" FILES ${SCENE_SOURCES})
source_group("Source Files\\vendor" FILES ${VENDOR_SOURCES})
source_group("Header Files" FILES ${MAIN_HEADERS})
source_group("Header Files\\scenes" FILES ${SCENE_HEADERS})
source_group("Header Files\\vendor" FILES ${VENDOR_HEADERS})

# Copy resources to output directory after build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    COMMENT "Copying resources to output directory..."
)

# Copy DLLs from libraries/dll to output directory
if(WIN32)
    file(GLOB DLL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/libraries/dll/*.dll")
    if(DLL_FILES)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${DLL_FILES}
                $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMENT "Copying DLLs to output directory..."
        )
    endif()
endif()