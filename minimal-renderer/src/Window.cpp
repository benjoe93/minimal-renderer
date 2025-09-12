#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Renderer.h"
#include "Window.h"

MainWindow::MainWindow()
{

    /* GLWF: init& config */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    /* GLWF: create a window and its OpenGL context */
    window = glfwCreateWindow(scr_widht, scr_height, "LearnOpenGL", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    AppState* app_state= new AppState{ renderer, true, 0.0f, 0.0f };
    glfwSetWindowUserPointer(window, app_state);

    // window resizing callback
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
        std::cout << "witdth: " << width << ", height: " << height << std::endl; 
    });

    // mouse position callback
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) 
    {
        AppState* state = static_cast<AppState*>(glfwGetWindowUserPointer(window));

        if (state->first_mouse)
        {
            state->last_x = static_cast<float>(xpos);
            state->last_y = static_cast<float>(ypos);
            state->first_mouse = false;
        }

        float xoffset = static_cast<float>(xpos) - state->last_x;
        float yoffset = static_cast<float>(ypos) - state->last_y;
        state->last_x = static_cast<float>(xpos);
        state->last_y = static_cast<float>(ypos);

        const float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        Camera* cam = state->renderer->m_active_cam;
        cam->SetYaw(cam->GetYaw() + xoffset);
        float new_pitch = cam->GetPitch() + -1.0f * yoffset;

        if (new_pitch > 89.0f)
        {
            new_pitch = 89.0f;
        }
        if (new_pitch < -89.0f)
        {
            new_pitch = -89.0f;
        }
        cam->SetPitch(new_pitch);

        cam->UpdateRotation(1.0f);
    });

    // mouse scroll callback
    glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset)
    {
        AppState* state = static_cast<AppState*>(glfwGetWindowUserPointer(window));

        float fov = state->renderer->m_active_cam->GetFov();
        fov -= static_cast<float>(yoffset);
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 90.0f)
            fov = 90.0f;
        state->renderer->m_active_cam->SetFov(fov);
    });

    /* GLAD: load all OpenGL function pointers */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
}

MainWindow::~MainWindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();    // glfw: terminate, clearing all previously allocated GLFW resources
}

void MainWindow::processInput()
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
        renderer->m_active_cam->MoveFwd(renderer->GetDeltaTime());
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && cursor_disabled)
    {
        renderer->m_active_cam->MoveBwd(renderer->GetDeltaTime());
    }
    // Move left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && cursor_disabled)
    {
        renderer->m_active_cam->MoveLeft(renderer->GetDeltaTime());
    }
    // Move right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && cursor_disabled)
    {
        renderer->m_active_cam->MoveRight(renderer->GetDeltaTime());
    }
    // Move down
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && cursor_disabled)
    {
        renderer->m_active_cam->MoveDown(renderer->GetDeltaTime());
    }
    // Move up
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && cursor_disabled)
    {
        renderer->m_active_cam->MoveUp(renderer->GetDeltaTime());
    }
}