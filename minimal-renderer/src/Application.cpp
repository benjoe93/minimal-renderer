#include <algorithm>
#include <iostream>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "Camera.h"
#include "EditorUI.h"
#include "ResourceManager.h"
#include "SceneRegistry.h"
#include "Viewport.h"

#define WINDOW_TITLE "LearnOpenGL"

constexpr float MOUSE_SENSITIVITY = 0.1f;
constexpr float SCROLL_SPEED_FACTOR = 5.0f;

/* glfw: whenever the window size changed(by OS or user resize) this callback function executes */
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

/* process all input : query GLFW whether relevant keys are pressed / released this frame and react accordingly */
void ProcessInput(GLFWwindow* window);

/* process mouse movement input */
void MouseCallback(GLFWwindow* window, double xpos, double ypos);

/* process mouse scroll input */
void ScrollCallback(GLFWwindow* window, double x_offset, double y_offset);

/* Update window title */
void UpdateWindowTitle(GLFWwindow* window, const std::string &scene_name, double delta_time);

int main()
{
    // GLFW: init& config
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    // GLFW: create a window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(1280, 720, WINDOW_TITLE, nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Bind input callbacks
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);    // window resizing callback
    glfwSetCursorPosCallback(window, MouseCallback);                    // mouse position callback
    glfwSetScrollCallback(window, ScrollCallback);                      // mouse scroll callback

    // GLAD: load all OpenGL function pointers
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    ResourceManager::Init();

    EditorUI editor_ui;
    editor_ui.Init(window);
    Viewport viewport;

    #ifndef NDEBUG
    // Test scene registration
    auto scene_names = SceneRegistry::Get().GetSceneNames();
    std::cout << "Registered scenes: " << scene_names.size() << std::endl;
    #endif

    Camera cam(0, glm::vec3(0.0f, 0.0f, 3.0f));
    auto& renderer = Renderer::Get();
    AppState::Get().RegisterCamera(cam);
    AppState::Get().SetActiveCamera(cam.GetId());
    glfwSetWindowUserPointer(window, &renderer);

    std::unique_ptr<Scene> current_scene = SceneRegistry::Get().CreateScene("00_ClearColor");

    /* RENDER LOOP */
    while (!glfwWindowShouldClose(window))
    {
        // render
        renderer.Tick(glfwGetTime());

        editor_ui.BeginFrame();
        editor_ui.Render(); // Menu bar and docking

        // Handle scene switching from menu
        std::string requested_scene = editor_ui.GetRequestedScene();
        if (!requested_scene.empty()) {
            current_scene = SceneRegistry::Get().CreateScene(requested_scene);
            editor_ui.ClearSceneRequest();
            renderer.SetBackgroundColor({ 0.1f, 0.1f, 0.1f, 1.0f });
            AppState::Get().GetActiveCamera().Reset();
        }

        ProcessInput(window);

        if (current_scene) {
            UpdateWindowTitle(window, current_scene->GetSceneName(), renderer.GetDeltaTime());

            viewport.Bind(); // render to framebuffer instead of screen
            renderer.Clear(); // clear the framebuffer

            current_scene->OnUpdate(renderer.GetDeltaTime());
            current_scene->OnRender();

            viewport.Unbind(); // back to screen
            current_scene->OnImGuiRender();
        }

        viewport.RenderImGuiWindow();
        editor_ui.EndFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    current_scene.reset();
    editor_ui.Shutdown();

    ResourceManager::Shutdown();

    /* glfw: terminate, clearing all previously allocated GLFW resources */
    glfwTerminate();
    return 0;
}

void UpdateWindowTitle(GLFWwindow* window, const std::string &scene_name, const double delta_time)
{
    char title[200];
    const double fps = 1.0 / delta_time;
    const double frame_time = delta_time * 1000.0;
    snprintf(title, sizeof(title), "%s / %s - FPS: %.1f | Frame: %.2fms", WINDOW_TITLE, scene_name.c_str(), fps, frame_time);
    glfwSetWindowTitle(window, title);
}

void FramebufferSizeCallback(GLFWwindow* window, const int width, const int height)
{
    const auto renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
    glViewport(0, 0, width, height);
    AppState::Get().SetScreenSize(width, height);
}

void ProcessInput(GLFWwindow* window)
{
    const auto renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
    Camera& cam = AppState::Get().GetActiveCamera();

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && AppState::Get().GetCursorEnabled())
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        AppState::Get().SetCursorState(CursorState::DISABLED);

    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && !AppState::Get().GetCursorEnabled())
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        AppState::Get().SetCursorState(CursorState::ENABLED);
    }

    // Exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // Move forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !AppState::Get().GetCursorEnabled())
    {
        cam.MoveFwd(renderer->GetDeltaTime());
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && !AppState::Get().GetCursorEnabled())
    {
        cam.MoveBwd(renderer->GetDeltaTime());
    }
    // Move left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && !AppState::Get().GetCursorEnabled())
    {
        cam.MoveLeft(renderer->GetDeltaTime());
    }
    // Move right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && !AppState::Get().GetCursorEnabled())
    {
        cam.MoveRight(renderer->GetDeltaTime());
    }
    // Move down
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && !AppState::Get().GetCursorEnabled())
    {
        cam.MoveDown(renderer->GetDeltaTime());
    }
    // Move up
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && !AppState::Get().GetCursorEnabled())
    {
        cam.MoveUp(renderer->GetDeltaTime());
    }
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    Camera& cam = AppState::Get().GetActiveCamera();

    if (AppState::Get().GetCursorEnabled()) {
        AppState::Get().SetCursorLastPosition(static_cast<int>(xpos), static_cast<int>(ypos));
    }

    const float last_x = AppState::Get().GetCursorLastPosition().x;
    const float last_y = AppState::Get().GetCursorLastPosition().y;
    const float x_offset = (static_cast<float>(xpos) - last_x) * MOUSE_SENSITIVITY;
    const float y_offset = (static_cast<float>(ypos) - last_y) * MOUSE_SENSITIVITY;

    AppState::Get().SetCursorLastPosition(static_cast<int>(xpos), static_cast<int>(ypos));

    cam.SetYaw(cam.GetYaw() + x_offset);
    cam.SetPitch(std::clamp(cam.GetPitch() + -1.0f * y_offset, -89.0f, 89.0f));
    cam.UpdateRotation();
}

void ScrollCallback(GLFWwindow* window, double x_offset, double y_offset)
{
    Camera& cam = AppState::Get().GetActiveCamera();
    cam.SetSpeed(std::clamp(cam.GetSpeed() + static_cast<float>(y_offset) / SCROLL_SPEED_FACTOR, 0.1f, 10.0f));
}
