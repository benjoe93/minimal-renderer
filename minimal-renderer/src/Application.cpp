#include <iostream>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Camera.h"
#include "ResourceManager.h"

#include "scenes/00_SceneClearColor.h"
#include "scenes/01_SceneHelloTriangle.h"
#include "scenes/02_SceneHelloTriangleIndexed.h"
#include "scenes/03_SceneTextures.h"
#include "scenes/04_SceneTransformation.h"
#include "scenes/05_SceneCoordinateSystem.h"
#include "scenes/06_SceneCube.h"
#include "scenes/07_SceneCubeMultiple.h"
#include "scenes/08_ScenePhongLight.h"
#include "scenes/09_SceneMaterials.h"
#include "scenes/10_SceneSurfMaps.h"
#include "scenes/11_SceneLightCasters.h"
#include "scenes/12_SceneMultipleLights.h"
#include "scenes/13_SceneModelLoading.h"
#include "scenes/14_SceneDepthTesting.h"
#include "scenes/15_SceneStencilTesting.h"
#include "scenes/16_SceneBlending.h"
#include "scenes/17_SceneFaceCulling.h"
#include "scenes/18_SceneFrameBuffers.h"
#include "scenes/19_SceneRearViewMirror.h"
#include "scenes/20_SceneCubemap.h"
#include "scenes/21_SceneAdvancedGLSL.h"


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
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

/* Update window title */
void UpdateWindowTitle(GLFWwindow* window, const std::string &scene_name, double delta_time);

int main(void)
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
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

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

    ResourceManager::Init();

    Camera cam(0, glm::vec3(0.0f, 0.0f, 3.0f));
    auto& renderer = Renderer::Get();
    renderer.GetState().cameras[cam.GetId()] = &cam;
    renderer.GetState().active_camera = cam.GetId();
    glfwSetWindowUserPointer(window, &renderer);

    auto scene = scene::SceneAdvancedGLSL();

    /* RENDER LOOP */
    while (!glfwWindowShouldClose(window))
    {
        // render
        renderer.Tick(glfwGetTime());
        renderer.Clear();
        
        UpdateWindowTitle(window, scene.GetSceneName(), renderer.GetDeltaTime());

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        //ImGui::ShowDemoWindow(); // Show demo window! :)

        ProcessInput(window);

        scene.OnUpdate(renderer.GetDeltaTime());
        scene.OnRender();
        scene.OnImGuiRender();
 
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

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
    renderer->SetScreenSize(width, height);
}

void ProcessInput(GLFWwindow* window)
{
    const auto renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
    Camera& cam = renderer->GetActiveCamera();

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && renderer->GetState().cursor_disabled == false)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        renderer->GetState().cursor_disabled = true;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && renderer->GetState().cursor_disabled)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        renderer->GetState().cursor_disabled = false;
    }

    // Exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // Move forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && renderer->GetState().cursor_disabled)
    {
        cam.MoveFwd(renderer->GetDeltaTime());
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && renderer->GetState().cursor_disabled)
    {
        cam.MoveBwd(renderer->GetDeltaTime());
    }
    // Move left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && renderer->GetState().cursor_disabled)
    {
        cam.MoveLeft(renderer->GetDeltaTime());
    }
    // Move right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && renderer->GetState().cursor_disabled)
    {
        cam.MoveRight(renderer->GetDeltaTime());
    }
    // Move down
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && renderer->GetState().cursor_disabled)
    {
        cam.MoveDown(renderer->GetDeltaTime());
    }
    // Move up
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && renderer->GetState().cursor_disabled)
    {
        cam.MoveUp(renderer->GetDeltaTime());
    }
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    const auto renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
    Camera& cam = renderer->GetActiveCamera();

    if (!renderer->GetState().cursor_disabled)
    {       
        renderer->GetState().last_x = static_cast<float>(xpos);
        renderer->GetState().last_y = static_cast<float>(ypos);
    }

    const float xoffset = (static_cast<float>(xpos) - renderer->GetState().last_x) * MOUSE_SENSITIVITY;
    const float yoffset = (static_cast<float>(ypos) - renderer->GetState().last_y) * MOUSE_SENSITIVITY;

    renderer->GetState().last_x = static_cast<float>(xpos);
    renderer->GetState().last_y = static_cast<float>(ypos);

    cam.SetYaw(cam.GetYaw() + xoffset);
    cam.SetPitch(std::clamp(cam.GetPitch() + -1.0f * yoffset, -89.0f, 89.0f));
    cam.UpdateRotation();
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    const auto renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
    Camera& cam = renderer->GetActiveCamera();

    cam.SetSpeed(std::clamp(cam.GetSpeed() + static_cast<float>(yoffset) / SCROLL_SPEED_FACTOR, 0.1f, 10.0f));
}
