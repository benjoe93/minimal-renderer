cmake_minimum_required(VERSION 3.16)
project(MinimalRenderer)

set(CMAKE_CXX_STANDARD 17)

# Output to bin/ folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add glad
add_library(glad src/glad.c)
target_include_directories(glad PUBLIC libs/glad/include)

# Add GLFW
add_subdirectory(libs/glfw)

# Executable
add_executable(minimal_renderer src/main.cpp)

# Link libs
target_link_libraries(minimal_renderer
    PRIVATE
        glad
        glfw
)

# Include paths
target_include_directories(minimal_renderer PRIVATE
    libs/glad/include
    libs/glfw/include
)
