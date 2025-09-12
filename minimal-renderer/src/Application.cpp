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
#include "Camera.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Renderer& renderer, bool& cursor_disabled);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

/* SETTINGS */
unsigned int SCR_WIDTH = 1280;
unsigned int SCR_HEIGHT = 720;

Camera* current_cam = nullptr;

bool cursor_disabled = false;

float last_x = 640;
float last_y = 360;

/* QUAD DEFINITION */
// Vertex array: 8 vertices with position (x,y,z), UV (u,v), and color (r,g,b)
// Each vertex has 8 floats: 3 position + 2 UV + 3 color
float vertices[] = {
    // positions            // uvs        // colors           
    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,   1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,   1.0f, 0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,

     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,    0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,    1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f,

    -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f
};
unsigned int indices[] = {
     0,  1,  2,
     3,  4,  5,
     6,  7,  8,
     9, 10, 11,
    12, 13, 14,
    15, 16, 17,
    18, 19, 20,
    21, 22, 23,
    24, 25, 26,
    27, 28, 29,
    30, 31, 32,
    33, 34, 35
};

/*float vertices[] = {
    // positions          // uvs        // colors           
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,   // 0: front top right
     0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   // 1: front bottom right
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   // 2: front bottom left
    -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,   1.0f, 1.0f, 0.0f,   // 3: front top left 

     0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   1.0f, 0.0f, 1.0f,   // 4: back top right
     0.5f, -0.5f, -0.5f,   1.0f, 0.0f,   0.0f, 1.0f, 1.0f,   // 5: back bottom right
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   1.0f, 0.0f, 1.0f,   // 6: back bottom left
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   1.0f, 1.0f, 1.0f    // 7: back top left 
};*/

// Index array: defines 12 triangles (2 per face) for 6 cube faces
// Using counter-clockwise winding order
//unsigned int indices[] = {
//    // Front face (z = +0.5)
//    0, 1, 2,
//    0, 2, 3,
//
//    // Back face (z = -0.5)
//    4, 7, 6,
//    4, 6, 5,
//
//    // Left face (x = -0.5)  
//    0, 3, 7,
//    0, 7, 4,
//
//    // Right face (x = +0.5)
//    1, 5, 6,
//    1, 6, 2,
//
//    // Bottom face (y = -0.5)
//    0, 4, 5,
//    0, 5, 1,
//
//    // Top face (y = +0.5)
//    3, 2, 6,
//    3, 6, 7
//};

// position vectors to render multiple cubes
glm::vec3 cube_positions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

int main(void)
{
    // GLWF: init& config
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    // GLWF: create a window and its OpenGL context 
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  // window resizing callback
    glfwSetCursorPosCallback(window, mouse_callback);                   // mouse position callback
    glfwSetScrollCallback(window, scroll_callback);                     // mouse scroll callback

    // GLAD: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Renderer renderer;
    renderer.SetBackgroundColor({ 0.2f, 0.3f, 0.3f, 1.0f });

    Camera camera(1, glm::vec3(0.0f, 0.0f, 3.0f));
    current_cam = &camera;
    renderer.m_active_cam = &camera;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true); // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init("#version 330");

    // Create textures
    Texture texture_1("resources/textures/container.jpg", GL_RGB);
    Texture texture_2("resources/textures/awesomeface.png", GL_RGBA);

    texture_1.Bind();
    texture_2.Bind(1);

    // Create shaders
    Shader default_shader("resources/shaders/default.vert", "resources/shaders/default.frag");

    // Pass textures to shader
    default_shader.Bind();
    default_shader.SetInt("texture_1", 0);
    default_shader.SetInt("texture_2", 1);

    // Crate VAO, VBO, EBO
    VertexArray va;
    va.Bind();
    VertexBuffer vb(vertices, 36 * 8 * sizeof(float));
    IndexBuffer ib(indices, 36);

    va.SetLayout(vb, 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);                    // vertex position
    va.SetLayout(vb, 1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));  // uv coords
    va.SetLayout(vb, 2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));  // vertex color

    vb.Unbind();
    va.Unbind();

    // Log OpenGL stats
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Maximum number of vertex attributes supported: " << renderer.GetMaxVertexAttribs() << std::endl;

    float offset[3] = { 0.0f, 0.0f, 0.0f };
    float rotation[3] = { 0.0f, 0.0f, 0.0f };
    bool show_demo_window = true;
    /* RENDER LOOP */
    while (!glfwWindowShouldClose(window))
    {
        // render
        renderer.Tick(glfwGetTime());
        renderer.Clear();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow(); // Show demo window! :)

        // input
        processInput(window, renderer, cursor_disabled);


        ImGui::Begin("My first window");
        ImGui::DragFloat3("Translate", offset, 0.01f);
        ImGui::DragFloat3("Rotation", rotation, 0.1f);
        ImGui::End();

        // Projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(camera.GetFov()), static_cast<float>(SCR_WIDTH)/ static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);
        
        // Model matrix
        glm::mat4 model = glm::mat4(1.0f);
        // transform with user input
        model = glm::translate(model, glm::vec3(offset[0], offset[1], offset[2]));
        model = glm::rotate(model, glm::radians(rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));

        default_shader.SetMat4("model", model);
        default_shader.SetMat4("view", camera.GetCurrentView());
        default_shader.SetMat4("projection", projection);

        // draw first triangle
        renderer.Draw(va, ib, default_shader);

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
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

/* process all input : query GLFW whether relevant keys are pressed / released this frame and react accordingly */
void processInput(GLFWwindow* window, Renderer& renderer, bool& cursor_disabled)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && cursor_disabled == false)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        cursor_disabled = true;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && cursor_disabled)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        cursor_disabled = false;
    }

    // Exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // Move forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && cursor_disabled)
    {
        renderer.m_active_cam->MoveFwd(renderer.GetDeltaTime());
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && cursor_disabled)
    {
        renderer.m_active_cam->MoveBwd(renderer.GetDeltaTime());
    }
    // Move left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && cursor_disabled)
    {
        renderer.m_active_cam->MoveLeft(renderer.GetDeltaTime());
    }
    // Move right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && cursor_disabled)
    {
        renderer.m_active_cam->MoveRight(renderer.GetDeltaTime());
    }
    // Move down
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && cursor_disabled)
    {
        renderer.m_active_cam->MoveDown(renderer.GetDeltaTime());
    }
    // Move up
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && cursor_disabled)
    {
        renderer.m_active_cam->MoveUp(renderer.GetDeltaTime());
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!cursor_disabled)
    {       
        last_x = static_cast<float>(xpos);
        last_y = static_cast<float>(ypos);
    }

    float xoffset = static_cast<float>(xpos) - last_x;
    float yoffset = static_cast<float>(ypos) - last_y;
    last_x = static_cast<float>(xpos);
    last_y = static_cast<float>(ypos);

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    current_cam->SetYaw(current_cam->GetYaw() + xoffset);
    float new_pitch = current_cam->GetPitch() + -1.0f * yoffset;

    if (new_pitch > 89.0f)
    {
        new_pitch = 89.0f;
    }
    if (new_pitch < -89.0f)
    {
        new_pitch = -89.0f;
    }
    current_cam->SetPitch(new_pitch);
    current_cam->UpdateRotation(1.0f);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    float fov = current_cam->GetFov();
    fov -= static_cast<float>(yoffset);
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 90.0f)
        fov = 90.0f;
    current_cam->SetFov(fov);
}