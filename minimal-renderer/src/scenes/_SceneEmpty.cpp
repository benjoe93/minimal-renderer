#include <glm/gtc/type_ptr.hpp>

#include "vendor/imgui/imgui.h"

#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "ResourceManager.h"

#include "_SceneEmpty.h"

#define OBJ_VERT_PATH "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.vert"
#define OBJ_FRAG_PATH "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.frag"

_SceneEmpty::_SceneEmpty()
    : Scene(StaticName())
{
    ConstructScene();
}

_SceneEmpty::~_SceneEmpty()
{
    for (Model* model : m_objects) {
        delete model;
    }
    m_objects.clear(); // Optional, but good practice
}

void _SceneEmpty::OnUpdate(double delta_time)
{
    auto floor = std::make_unique<Model>("resources/models/plane.fbx");
    floor->SetLocation({0.0f, -0.5f, 0.0f});
    floor->SetRotation({-90.0f, 0.0f, 0.0f});
    auto box1 = std::make_unique<Model>("resources/models/box.fbx");
    box1->SetLocation({-1.5f, 0.0f, -1.0f});
    auto box2 = std::make_unique<Model>("resources/models/box.fbx");
    box2->SetLocation({1.5f, 0.0f, 0.0f});

    Material* floor_material = ResourceManager::Get().CreateMaterial("floor_material", OBJ_VERT_PATH, OBJ_FRAG_PATH);
    Material* box_material = ResourceManager::Get().CreateMaterial("box_material", OBJ_VERT_PATH, OBJ_FRAG_PATH);
    floor_material->AddTexture2D("resources/textures/metal.png", "material.diffuse");
    box_material->AddTexture2D("resources/textures/container.jpg", "material.diffuse");

    floor->SetMaterialSlot(0, floor_material);
    box1->SetMaterialSlot(0, box_material);
    box2->SetMaterialSlot(0, box_material);

    m_objects.push_back(std::move(floor));
    m_objects.push_back(std::move(box1));
    m_objects.push_back(std::move(box2));
}
