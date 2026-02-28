#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "Camera.h"
#include "IndexBuffer.h"
#include "Texture2D.h"
#include "Material.h"
#include "Model.h"
#include "ResourceManager.h"

#include "16_SceneBlending.h"

#include <ranges>

#include "SceneRegistry.h"

#define OBJ_VERT_PATH "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.vert"
#define OBJ_FRAG_PATH "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.frag"

#define TRANS_VERT_PATH "resources/shaders/03_AdvancedOpenGL/03_Blending/window.vert"
#define TRANS_FRAG_PATH "resources/shaders/03_AdvancedOpenGL/03_Blending/window.frag"

#define MASK_VERT_PATH "resources/shaders/03_AdvancedOpenGL/03_Blending/grass.vert"
#define MASK_FRAG_PATH "resources/shaders/03_AdvancedOpenGL/03_Blending/grass.frag"

SceneBlending::SceneBlending()
    : Scene(StaticName())
{
    Renderer::Get().SetFaceCulling(false);

    window_loc.emplace_back(-1.5f, 0.0f, -0.48f);
    window_loc.emplace_back( 1.5f, 0.0f,  0.51f);
    window_loc.emplace_back( 0.0f, 0.0f,  0.7f);
    window_loc.emplace_back(-0.3f, 0.0f, -2.3f);
    window_loc.emplace_back( 0.5f, 0.0f, -0.6f);

    vegetation_loc.emplace_back( 0.5f, -0.25f,  0.48f);
    vegetation_loc.emplace_back(-2.5f, -0.25f, -0.51f);
    vegetation_loc.emplace_back( 0.0f, -0.25f,  0.2f);

    // geometry setup
    auto floor = std::make_unique<Model>("resources/models/plane.fbx");
    floor->SetLocation({0.0f, -0.5f, 0.0f});
    floor->SetRotation({-90.0f, 0.0f, 0.0f});
    auto box1 = std::make_unique<Model>("resources/models/box.fbx");
    box1->SetLocation({-1.5f, 0.0f, -1.0f});
    auto box2 = std::make_unique<Model>("resources/models/box.fbx");
    box2->SetLocation({1.5f, 0.0f, 0.0f});

    auto floor_material = ResourceManager::Get().GetMaterial("floor_material");
    if (!floor_material) {
        floor_material = ResourceManager::Get().CreateMaterial("floor_material", OBJ_VERT_PATH, OBJ_FRAG_PATH);
        floor_material->AddTexture2D("resources/textures/metal.png", "material.diffuse");
    }
    auto box_material = ResourceManager::Get().GetMaterial("box_material");
    if (!box_material) {
        box_material = ResourceManager::Get().CreateMaterial("box_material", OBJ_VERT_PATH, OBJ_FRAG_PATH);
        box_material->AddTexture2D("resources/textures/container.jpg", "material.diffuse");
    }

    floor->SetMaterialSlot(0, floor_material);
    box1->SetMaterialSlot(0, box_material);
    box2->SetMaterialSlot(0, box_material);

    objects.push_back(std::move(floor));
    objects.push_back(std::move(box1));
    objects.push_back(std::move(box2));

    // windows
    auto window_material = ResourceManager::Get().GetMaterial("window");
    if (!window_material) {
        window_material = ResourceManager::Get().CreateMaterial("window", TRANS_VERT_PATH, TRANS_FRAG_PATH);
        window_material->AddTexture2D("resources/textures/window.png", "material.diffuse");
    }

    for (auto location : window_loc)
    {
        auto window = std::make_unique<Model>("resources/models/plane.fbx");
        window->SetLocation(location);
        window->SetRotation({0.0f, 0.0f, 180.0f});
        window->SetScale(glm::vec3(0.1f));
        window->SetMaterialSlot(0, window_material);
        transparent_objects.push_back(std::move(window));
    }

    // grass
    auto grass_material = ResourceManager::Get().GetMaterial("grass");
    if (!grass_material) {
        grass_material = ResourceManager::Get().CreateMaterial("grass", MASK_VERT_PATH, MASK_FRAG_PATH);
        auto grass_tex = ResourceManager::Get().GetTexture2D("resources/textures/grass.png", true);
        grass_tex->SetWrappingHorizontal(GL_CLAMP_TO_EDGE);
        grass_tex->SetWrappingVertical(GL_CLAMP_TO_EDGE);
        grass_material->AddTexture("material.diffuse", grass_tex);
    }

    for (auto location : vegetation_loc)
    {
        auto grass = std::make_unique<Model>("resources/models/plane.fbx");
        grass->SetLocation(location);
        grass->SetRotation({0.0f, 0.0f, 180.0f});
        grass->SetScale(glm::vec3(0.1f));
        grass->SetMaterialSlot(0, grass_material);
        objects.push_back(std::move(grass));
    }
}

void SceneBlending::OnUpdate(double delta_time) {
    Renderer::Get().SetBackgroundColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
}

void SceneBlending::OnRender()
{
    // sort transparent objects based on distance from camera
    std::map<float, Model*> sorted;
    for (const auto & transparent_object : transparent_objects)
    {
        float distance = glm::length(AppState::Get().GetActiveCamera().GetPosition() - transparent_object->GetLocation());
        sorted[distance] = transparent_object.get();
    }

    Camera& cam = AppState::Get().GetActiveCamera();
    glm::mat4 projection, ModelView, MVP;
    projection = glm::perspective(
        glm::radians(cam.GetFov()),
        static_cast<float>(AppState::Get().GetScreenWidth()) / static_cast<float>(AppState::Get().GetScreenHeight()),
        AppState::Get().GetNearPlane(),
        AppState::Get().GetFarPlane()
    );

    for (auto& obj : objects) {
        ModelView = cam.GetViewMatrix() * obj->GetModelMatrix();
        MVP = projection * ModelView;

        for (auto mat : obj->GetAllMaterials()) {
            mat->SetUniform("mvp", MVP);
        }

        Renderer::Get().Draw(*obj);
    }

    // render object in reverse distance order
    for (auto obj : sorted | std::views::values | std::views::reverse) {
        ModelView = cam.GetViewMatrix() * obj->GetModelMatrix();
        MVP = projection * ModelView;

        for (auto mat : obj->GetAllMaterials()) {
            mat->SetUniform("mvp", MVP);
        }

        Renderer::Get().Draw(*obj);
    }
}

void SceneBlending::OnImGuiRender() { }

REGISTER_SCENE(SceneBlending);