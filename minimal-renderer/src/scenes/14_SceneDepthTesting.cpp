#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "Camera.h"
#include "Material.h"
#include "Model.h"
#include "ResourceManager.h"

#include "14_SceneDepthTesting.h"

#include "SceneRegistry.h"

#define PLANE_PATH "resources/models/plane.fbx"
#define BOX_PATH "resources/models/box.fbx"

#define OBJ_VERT_PATH "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.vert"
#define OBJ_FRAG_PATH "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.frag"

SceneDepthTesting::SceneDepthTesting()
    : Scene(StaticName())
{
    Renderer::Get().SetBackgroundColor(glm::vec4(0.18f, 0.23f, 0.24f, 1.0f));

    Camera& camera = AppState::Get().GetActiveCamera();

    // geometry setup
    auto floor = std::make_unique<Model>(PLANE_PATH);
    floor->SetLocation({0.0f, -0.5f, 0.0f});
    floor->SetRotation({-90.0f, 0.0f, 0.0f});

    auto box1 = std::make_unique<Model>(BOX_PATH);
    box1->SetLocation({-1.5f, 0.0f, -1.0f});

    auto box2 = std::make_unique<Model>(BOX_PATH);
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

}

void SceneDepthTesting::OnUpdate(double delta_time) {}

void SceneDepthTesting::OnRender() {
    Renderer::Get().SetBackgroundColor(glm::vec4(0.18f, 0.23f, 0.24f, 1.0f));

    Camera& cam = AppState::Get().GetActiveCamera();
    glm::vec3 cam_pos = cam.GetPosition();

    glm::mat4 projection, model, ModelView, MVP;
    projection = glm::perspective(
        glm::radians(
            cam.GetFov()),
            static_cast<float>(AppState::Get().GetScreenWidth()) / static_cast<float>(AppState::Get().GetScreenHeight()),
        0.1f,
        100.0f);

    // geometry rendering -  we're doing this here to have the mvp updated right before the draw call even if multiple
    // objects are using the same material
    for (auto& obj : objects) {
        for (auto& material : obj->GetAllMaterials()) {
            model = obj->GetModelMatrix();
            ModelView = cam.GetViewMatrix() * model;
            MVP = projection * ModelView;

            if (!material) continue;
            material->SetUniform("model", model);
            material->SetUniform("mvp", MVP);
        }
        Renderer::Get().Draw(*obj);
    }
}

void SceneDepthTesting::OnImGuiRender() {}

REGISTER_SCENE(SceneDepthTesting);