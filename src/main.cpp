#include <glad/glad.h>      // GLAD: Loads OpenGL function pointers
#include <GLFW/glfw3.h>     // GLFW: Window/context/input handling
#include <iostream>
#include <string>
#include <fstream>          // used for reading files

// Loads the shader code from file into memory
static std::string LoadShaderSource(const std::string& filepath)
{
    std::ifstream file (filepath, std::ios::binary);

    if (!file)
    {
        throw std::runtime_error("Cannot open file :" + filepath);
    }

    // Read entire file into string using iterators
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    if (file.bad())
    {
        throw std::runtime_error("Error reading file: " + filepath);
    }

    return content;
}

// Compiles shader
static GLuint CompileShader(const std::string& shaderSource, GLenum type)
{
    GLuint shader = glCreateShader(type);

    const char* source = shaderSource.c_str();      // Get C string from std::string
    glShaderSource(shader, 1, &source, nullptr);    // 1 string, nullptr for auto length
    glCompileShader(shader);

    // Error checking
    std::string shaderTypeStr =
        (type == GL_VERTEX_SHADER) ? "VERTEX" :
        (type == GL_FRAGMENT_SHADER) ? "FRAGMENT" : "UNKNOWN";

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[1024];                       // Bigger buffer, should handle most cases
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        std::cerr << shaderTypeStr << " Shader compilation failed: " << infoLog << std::endl;
    }

    return shader;
}

// Create shader program and links shaders
static GLuint CreateShaderProgram(const std::string& vertexSource, const std::string& fragmentSource)
{
    // Compile shaders
    GLint vertShader = CompileShader(vertexSource, GL_VERTEX_SHADER);
    GLint fragShader = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);

    // Create shader program and link shaders
    GLint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);

    // Link shader program
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success)
    {
        GLchar infoLog[1024];
        glGetProgramInfoLog(program, 1024, nullptr, infoLog);
        std::cerr << "Program linking failed: " << infoLog << std::endl;
    }

    // Delete shader
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return program;
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Set OpenGL context version to 4.5 and use core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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
        // Clear the screen with chosen color
        glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    // Cleanup and exit
    glfwDestroyWindow(Window);
    glfwTerminate();
    return 0;
}
