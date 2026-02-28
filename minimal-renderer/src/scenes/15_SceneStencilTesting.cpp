#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "Camera.h"
#include "Material.h"
#include "Model.h"
#include "ResourceManager.h"

#include "15_SceneStencilTesting.h"

#include <ranges>

#include "SceneRegistry.h"

#define OBJ_VERT_PATH "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.vert"
#define OBJ_FRAG_PATH "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.frag"

#define STENCIL_VERT_PATH "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/stencil_test.vert"
#define STENCIL_FRAG_PATH "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/stencil_test.frag"


SceneStencilTesting::SceneStencilTesting()
    : Scene(StaticName())
{
    Renderer::Get().SetBackgroundColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

    Renderer::Get().SetDepthTest(true);
    Renderer::Get().SetDepthFunction(TestingFunc::LESS);
    Renderer::Get().SetStencilTest(true);
    Renderer::Get().SetStencilFunction(TestingFunc::NOTEQUAL, 1, 0xFF);
    Renderer::Get().SetStencilOperation(StencilOp::KEEP, StencilOp::KEEP, StencilOp::REPLACE);

    Camera& camera = AppState::Get().GetActiveCamera();

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

    // boxes for the outline
    auto outline_box1 = std::make_unique<Model>("resources/models/box.fbx");
    outline_box1->SetLocation({-1.5f, 0.0f, -1.0f});
    outline_box1->SetScale(glm::vec3(1.1f));
    auto outline_box2 = std::make_unique<Model>("resources/models/box.fbx");
    outline_box2->SetLocation({1.5f, 0.0f, 0.0f});
    outline_box2->SetScale(glm::vec3(1.1f));

    outline_objects.push_back(std::move(outline_box1));
    outline_objects.push_back(std::move(outline_box2));

    auto outline_mat = ResourceManager::Get().GetMaterial("outline_mat");
    if (!outline_mat) {
        outline_mat = ResourceManager::Get().CreateMaterial("outline_mat", STENCIL_VERT_PATH, STENCIL_FRAG_PATH);
    }

    for (auto& obj : outline_objects) {
        obj->SetMaterialSlot(0, outline_mat);
    }
}

void SceneStencilTesting::OnUpdate(double delta_time)
{
    Renderer::Get().SetBackgroundColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
}

void SceneStencilTesting::OnRender()
{
    Camera& cam = AppState::Get().GetActiveCamera();

    glm::mat4 projection, model, ModelView, MVP;
    projection = glm::perspective(
        glm::radians(
            cam.GetFov()),
            static_cast<float>(AppState::Get().GetScreenWidth()) / static_cast<float>(AppState::Get().GetScreenHeight()),
        0.1f,
        100.0f);

    // Draw floor
    Renderer::Get().SetDepthTest(true);
    Renderer::Get().SetStencilOperation(StencilOp::KEEP, StencilOp::KEEP, StencilOp::REPLACE);
    Renderer::Get().SetStencilMask(0x00);

    model = objects[0]->GetModelMatrix();
    ModelView = cam.GetViewMatrix() * model;
    MVP = projection * ModelView;

    for (auto& mat : objects[0]->GetAllMaterials()) {
        mat->SetUniform("model", model);
        mat->SetUniform("mvp", MVP);
    }
    Renderer::Get().Draw(*objects[0]);

    // Draw boxes
    Renderer::Get().SetStencilFunction(TestingFunc::ALWAYS, 1, 0xFF);
    Renderer::Get().SetStencilMask(0xFF);
    for (auto& obj : objects | std::views::drop(1)) {
        model = obj->GetModelMatrix();
        ModelView = cam.GetViewMatrix() * model;
        MVP = projection * ModelView;

        for (auto& mat : obj->GetAllMaterials()) {
            mat->SetUniform("model", model);
            mat->SetUniform("mvp", MVP);
        }

        Renderer::Get().Draw(*obj);
    }

    // Draw outlines
    Renderer::Get().SetStencilFunction(TestingFunc::NOTEQUAL, 1, 0xFF);
    Renderer::Get().SetStencilMask(0x00);
    Renderer::Get().SetDepthTest(false);

    for (auto& obj : outline_objects) {
        model = obj->GetModelMatrix();
        ModelView = cam.GetViewMatrix() * model;
        MVP = projection * ModelView;

        for (auto& mat : obj->GetAllMaterials()) {
            mat->SetUniform("model", model);
            mat->SetUniform("mvp", MVP);
        }

        Renderer::Get().Draw(*obj);
    }

    Renderer::Get().SetStencilMask(0xFF);
    Renderer::Get().SetStencilFunction(TestingFunc::ALWAYS, 0, 0xFF);
    Renderer::Get().SetDepthTest(true);
}

void SceneStencilTesting::OnImGuiRender() { }

REGISTER_SCENE(SceneStencilTesting);