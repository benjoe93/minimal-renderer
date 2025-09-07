#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Renderer.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

/* SETTINGS */
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

/* QUAD DEFINITION */
float vertices[] = {
    // positions          // uvs        // colors           
     0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,   // top right
     0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f,   1.0f, 1.0f, 0.0f    // top left 
};
unsigned int indices[] = {
    0, 1, 3,    // first triangle
    1, 2, 3,    // second triangle
};

int main(void)
{
    /* GLWF: init& config */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    /* GLWF: create a window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /* GLAD: load all OpenGL function pointers */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /* ImGui Setup */
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Create textures
    Texture texture1("resources/textures/container.jpg", GL_RGB);
    Texture texture2("resources/textures/awesomeface.png", GL_RGBA);

    texture1.Bind();
    texture2.Bind(1);

    // Create shaders
    Shader defaultShader("resources/shaders/default.vert", "resources/shaders/default.frag");

    // Pass textures to shader
    defaultShader.Bind();
    defaultShader.SetInt("texture1", 0);
    defaultShader.SetInt("texture2", 1);

    // Crate VAO, VBO, EBO
    VertexArray vao1;
    vao1.Bind();
    VertexBuffer vbo1(vertices, 4 * 8 * sizeof(float));
    IndexBuffer ebo1(indices, 6);

    vao1.SetLayout(vbo1, 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);                    // vertex position
    vao1.SetLayout(vbo1, 1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));  // uv coords
    vao1.SetLayout(vbo1, 2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));  // vertex color

    vbo1.Unbind();
    vao1.Unbind();

    Renderer renderer;
    renderer.SetBackgroundColor({ 0.2f, 0.3f, 0.3f, 1.0f });
    renderer.ToggleWireframeRender(false);

    // Log OpenGL stats
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Maximum number of vertex attributes supported: " << renderer.GetMaxVertexAttribs() << std::endl;

    float offset[3] = { 0.0f, 0.0f, 0.0f };
    float rotation[3] = { 0.0f, 0.0f, 0.0f };
    /* RENDER LOOP */
    while (!glfwWindowShouldClose(window))
    {
        // render
        renderer.Clear();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // input
        processInput(window);

        ImGui::Begin("My first window");
        ImGui::DragFloat3("Translate", offset, 0.01f, -1.0f, 1.0f);
        ImGui::DragFloat3("Rotation", rotation, 0.01f);
        ImGui::End();

        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(offset[0], offset[1], offset[2]));
        //trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::rotate(trans, glm::radians(rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
        trans = glm::rotate(trans, glm::radians(rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
        trans = glm::rotate(trans, glm::radians(rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));


        defaultShader.SetMat4("transform", trans);

        // draw first triangle
        renderer.Draw(vao1, ebo1, defaultShader);



        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);

    /* glfw: terminate, clearing all previously allocated GLFW resources */
    glfwTerminate();
    return 0;
}


/* glfw: whenever the window size changed(by OS or user resize) this callback function executes */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/* process all input : query GLFW whether relevant keys are pressed / released this frame and react accordingly */
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}