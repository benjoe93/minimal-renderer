#include <glad/glad.h>    // GLAD: Loads OpenGL function pointers
#include <GLFW/glfw3.h>   // GLFW: Window/context/input handling
#include <iostream>

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Set OpenGL context version to 4.5 and use core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    // Optional - Not resizable window
    // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
 
    // Create a windowed mode window and OpenGL context
    GLFWwindow* Window = glfwCreateWindow(800, 600, "Minimal Renderer", nullptr, nullptr);
    if (!Window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    // Make the OpenGL context current for this thread
    glfwMakeContextCurrent(Window);

    // Load OpenGL functions using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // Print out the loaded OpenGL version (just to test it worked)
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";

    // Main render loop
    while (!glfwWindowShouldClose(Window)) {
        // Here you'd normally render your scene
        // Right now it's just a blank window
    
        glfwSwapBuffers(Window);    // Swap front and back buffers
        glfwPollEvents();           // Handle input/events
    }

    // Cleanup and exit
    glfwDestroyWindow(Window);
    glfwTerminate();
    return 0;
}
