#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "Camera.h"
#include "Material.h"
#include "Model.h"
#include "ResourceManager.h"

#include "17_SceneFaceCulling.h"

#include "SceneRegistry.h"

#define OBJ_VERT_PATH "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.vert"
#define OBJ_FRAG_PATH "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.frag"

SceneFaceCulling::SceneFaceCulling()
    : Scene(StaticName())
{
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
}

void SceneFaceCulling::OnUpdate(double delta_time) {
    Renderer::Get().SetBackgroundColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
}

void SceneFaceCulling::OnRender() {
    Camera& cam = AppState::Get().GetActiveCamera();
    glm::mat4 projection, ModelView, MVP;
    projection = glm::perspective(
        glm::radians(cam.GetFov()),
        static_cast<float>(AppState::Get().GetScreenWidth()) / static_cast<float>(AppState::Get().GetScreenHeight()),
        AppState::Get().GetNearPlane(),
        AppState::Get().GetFarPlane());


    for (auto& obj : objects) {
        ModelView = cam.GetViewMatrix() * obj->GetModelMatrix();
        MVP = projection * ModelView;

        for (auto mat : obj->GetAllMaterials()) {
            mat->SetUniform("mvp", MVP);
        }

        Renderer::Get().Draw(*obj);
    }
}

void SceneFaceCulling::OnImGuiRender() { }

REGISTER_SCENE(SceneFaceCulling);