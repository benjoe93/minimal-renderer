#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "vendor/imgui/imgui.h"

#include "Renderer.h"
#include "Camera.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "TextureCubemap.h"
#include "ResourceManager.h"

#include "20_SceneCubemap.h"

#include <ranges>

#include "SceneRegistry.h"

#define OBJ_VERT_PATH "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.vert"
#define OBJ_FRAG_PATH "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.frag"

#define CUBE_VERT_PATH "resources/shaders/03_AdvancedOpenGL/05_Cubemap/cubemap.vert"
#define CUBE_FRAG_PATH "resources/shaders/03_AdvancedOpenGL/05_Cubemap/cubemap.frag"

#define REFLECTION_VERT_PATH "resources/shaders/03_AdvancedOpenGL/05_Cubemap/reflection.vert"
#define REFLECTION_FRAG_PATH "resources/shaders/03_AdvancedOpenGL/05_Cubemap/reflection.frag"

#define REFRACTION_VERT_PATH "resources/shaders/03_AdvancedOpenGL/05_Cubemap/reflection.vert"
#define REFRACTION_FRAG_PATH "resources/shaders/03_AdvancedOpenGL/05_Cubemap/refraction.frag"

static std::unordered_map<CubeSide, std::string> textures_faces = {
    { CubeSide::FRONT,    "resources/textures/skybox/front.jpg", },
    { CubeSide::BACK,     "resources/textures/skybox/back.jpg", },
    { CubeSide::TOP,      "resources/textures/skybox/top.jpg", },
    { CubeSide::BOTTOM,   "resources/textures/skybox/bottom.jpg", },
    { CubeSide::LEFT,     "resources/textures/skybox/left.jpg", },
    { CubeSide::RIGHT,    "resources/textures/skybox/right.jpg" }
};
static std::vector<float> skybox_verts = {
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

SceneCubemap::SceneCubemap()
    : Scene(StaticName())
{
    skybox_va = std::make_unique<VertexArray>();
    skybox_va->Bind();

    skybox_vb = std::make_unique<VertexBuffer>(skybox_verts.data(), skybox_verts.size() * sizeof(float));
    skybox_vb->Bind();

    skybox_va->SetLayout(*skybox_vb, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    skybox_material = ResourceManager::Get().GetMaterial("skybox_material");
    if (!skybox_material) {
        skybox_material = ResourceManager::Get().CreateMaterial("skybox_material", CUBE_VERT_PATH, CUBE_FRAG_PATH);
        cubemap = ResourceManager::Get().GetCubemap(textures_faces);
        skybox_material->AddTexture("cubemap", cubemap);
    }

    ConstructScene();
}

void SceneCubemap::OnUpdate(double delta_time)
{
    Renderer::Get().SetBackgroundColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

    switch (m_active_mode) {
        case 0:
            SetNormalScene();
            break;
        case 1:
            SetReflectionScene();
            break;
        case 2:
            SetRefractionScene();
            break;
    }
}

void SceneCubemap::OnRender()
{
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

        for (Material* mat : obj->GetAllMaterials()) {
            mat->SetUniform("mvp", MVP);
            mat->SetUniform("model", obj->GetModelMatrix());
            mat->SetUniform("camera_position", cam.GetPosition());
        }

        Renderer::Get().Draw(*obj);
    }

    // Optimization: Rendering skybox after the scene is done so fragments covered by the scene doesn't need to be re-rendered.
    glm::mat4 view = glm::mat4(glm::mat3(cam.GetViewMatrix())); // removes translation
    MVP = projection * view;
    skybox_material->SetUniform("mvp", MVP);

    Renderer::Get().SetDepthFunction(TestingFunc::LEQUAL);
    Renderer::Get().Draw(*skybox_va, static_cast<int>(skybox_verts.size()), skybox_material);
    Renderer::Get().SetDepthFunction(TestingFunc::LESS);
}

void SceneCubemap::OnImGuiRender()
{
    ImGui::Begin(m_name.c_str());

    ImGui::Combo("View Mode", &m_active_mode, m_view_modes, 3);

    ImGui::End();
}

void SceneCubemap::ConstructScene()
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

void SceneCubemap::SetNormalScene() {
    auto floor_material = ResourceManager::Get().GetMaterial("floor_material");
    auto box_material = ResourceManager::Get().GetMaterial("box_material");

    objects[0]->SetMaterialSlot(0, floor_material);
    for (auto& obj : objects | std::views::drop(1)) {
        obj->SetMaterialSlot(0, box_material);
    }
}

void SceneCubemap::SetReflectionScene()
{
    auto reflection_material = ResourceManager::Get().GetMaterial("reflection_material");
    if (!reflection_material) {
        reflection_material = ResourceManager::Get().CreateMaterial("reflection_material", REFLECTION_VERT_PATH, REFLECTION_FRAG_PATH);
        reflection_material->AddTexture("cubemap", cubemap);
    }

    for (auto& obj : objects) {
        obj->SetMaterialSlot(0, reflection_material);
    }
}

void SceneCubemap::SetRefractionScene()
{
    auto refraction_material = ResourceManager::Get().GetMaterial("refraction_material");
    if (!refraction_material) {
        refraction_material = ResourceManager::Get().CreateMaterial("refraction_material", REFRACTION_VERT_PATH, REFRACTION_FRAG_PATH);
        refraction_material->AddTexture("cubemap", cubemap);
    }

    for (auto& obj : objects) {
        obj->SetMaterialSlot(0, refraction_material);
    }
}

REGISTER_SCENE(SceneCubemap);