# Specify minimum CMake version required
cmake_minimum_required(VERSION 3.16)

# Define the project name (this becomes the solution name)
project(MinimalRenderer)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # Make C++17 required, not optional

# Configure output directories
# This puts all executables in build/bin/ folder instead of scattered locations
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ============================================================================
# GLAD Library Setup
# ============================================================================
# GLAD is an OpenGL loader library that helps load OpenGL functions
add_library(glad src/glad.c)

# Make GLAD's include directory available to any target that links to it
target_include_directories(glad PUBLIC libs/glad/include)

# Hide GLAD from Visual Studio solution
set_target_properties(glad PROPERTIES FOLDER "External Libraries")

# ============================================================================
# GLFW Library Setup
# ============================================================================
# GLFW provides window creation and input handling for OpenGL
# This adds the entire GLFW project from the libs/glfw subdirectory
add_subdirectory(libs/glfw)

# Optional: Disable GLFW examples, tests, and docs to speed up build
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

# Hide GLFW targets from Visual Studio solution
set_target_properties(glfw PROPERTIES FOLDER "External Libraries")
set_target_properties(update_mappings PROPERTIES FOLDER "External Libraries")

# ============================================================================
# Main Executable
# ============================================================================
# Create the main executable from your source files
add_executable(minimal_renderer src/main.cpp)

# Link required libraries to your executable
target_link_libraries(minimal_renderer
    PRIVATE                    # These libraries are only needed internally
        glad                   # OpenGL loader
        glfw                   # Window and input handling
)

# Add include directories for your executable
target_include_directories(minimal_renderer PRIVATE
    libs/glad/include         # For glad/glad.h
    libs/glfw/include         # For GLFW/glfw3.h
)

# ============================================================================
# Platform-specific settings
# ============================================================================
# On Windows, link additional system libraries that OpenGL needs
if(WIN32)
    target_link_libraries(minimal_renderer PRIVATE opengl32)
endif()

# On Linux, you might need these libraries
if(UNIX AND NOT APPLE)
    find_package(OpenGL REQUIRED)
    target_link_libraries(minimal_renderer PRIVATE OpenGL::GL)
endif()

# ============================================================================
# Compiler-specific settings
# ============================================================================
# Set some useful compiler flags
if(MSVC)
    # Visual Studio specific settings
    target_compile_options(minimal_renderer PRIVATE /W4)  # Warning level 4
else()
    # GCC/Clang settings
    target_compile_options(minimal_renderer PRIVATE -Wall -Wextra)
endif()

# ============================================================================
# Shader Management - Simple Approach
# ============================================================================
# Create a simple custom target that copies shaders during build
add_custom_target(copy_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/bin/shaders"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/shaders" "${CMAKE_BINARY_DIR}/bin/shaders"
    COMMENT "Copying shaders to build/bin/shaders"
    VERBATIM
)

# Hide the copy_shaders utility target from Visual Studio solution
set_target_properties(copy_shaders PROPERTIES FOLDER "Build Utilities")

# Make sure shaders are copied before the main executable runs
add_dependencies(minimal_renderer copy_shaders)

# Try to add shader files to Visual Studio solution (if they exist)
if(EXISTS "${CMAKE_SOURCE_DIR}/shaders")
    file(GLOB_RECURSE SHADER_FILES "${CMAKE_SOURCE_DIR}/shaders/*")
    
    # Only add to solution if we found shader files
    if(SHADER_FILES)
        # Create a custom target to hold shader files in Visual Studio
        add_custom_target(Shaders SOURCES ${SHADER_FILES})
        
        # Group shader files in Visual Studio
        source_group("Shaders" FILES ${SHADER_FILES})
        
        # Keep the Shaders project visible but organize it
        set_target_properties(Shaders PROPERTIES FOLDER "Source Files")
    endif()
endif()

# ============================================================================
# Asset Management
# ============================================================================
# Copy assets to output directory if they exist
if(EXISTS "${CMAKE_SOURCE_DIR}/assets")
    file(COPY "${CMAKE_SOURCE_DIR}/assets" 
         DESTINATION "${CMAKE_BINARY_DIR}/bin")
endif()
