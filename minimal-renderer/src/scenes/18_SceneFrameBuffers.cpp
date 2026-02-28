#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "vendor/imgui/imgui.h"

#include "Renderer.h"
#include "Camera.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "FrameBuffer.h"
#include "RenderTarget.h"
#include "ResourceManager.h"

#include "18_SceneFrameBuffers.h"

#include "SceneRegistry.h"

#define OBJ_VERT_PATH "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.vert"
#define OBJ_FRAG_PATH "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.frag"

#define BUFFER_VERT_PATH "resources/shaders/03_AdvancedOpenGL/04_Framebuffer/framebuffer.vert"
#define BUFFER_FRAG_PATH "resources/shaders/03_AdvancedOpenGL/04_Framebuffer/framebuffer.frag"

static std::vector<Vertex> verts = {
           // positions          // normals          // texCoords
    Vertex({-1.0f,  1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}), // top left
    Vertex({-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}), // bot left
    Vertex({ 1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}), // bot right
    Vertex({ 1.0f,  1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}), // top right
};
static std::vector<unsigned int> indices = {
    0, 1, 2,
    0, 2, 3
};

SceneFramebuffer::SceneFramebuffer()
    : Scene(StaticName())
{
    // framebuffer configuration
    framebuffer = std::make_unique<Framebuffer>();

    // create a color attachment texture
    render_target = new RenderTarget(AppState::Get().GetScreenWidth(), AppState::Get().GetScreenHeight(), 3);
    framebuffer->AttachRenderTarget(AttachmentTarget::COLOR0, render_target);

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    render_buffer = new RenderBuffer(AppState::Get().GetScreenWidth(), AppState::Get().GetScreenHeight());
    framebuffer->AttachRenderBuffer(AttachmentTarget::DEPTH_STENCIL, render_buffer);
    
    // unbind to prevent accidental renders
    framebuffer->Unbind();

    // create mesh list
    std::vector<std::unique_ptr<Mesh>> meshes;
    meshes.push_back(std::make_unique<Mesh>(verts, indices));

    // create model
    quad_normal = std::make_unique<Model>(
        std::move(meshes),
        Transform()
    );

    auto quad_material = ResourceManager::Get().GetMaterial("quad_material");
    if (!quad_material) {
        quad_material = ResourceManager::Get().CreateMaterial("quad_material", BUFFER_VERT_PATH, BUFFER_VERT_PATH);
        quad_material->AddTexture("screenTexture", render_target);
    }
    quad_normal->SetMaterialSlot(0, quad_material);

    ConstructScene();
}

SceneFramebuffer::~SceneFramebuffer()
{
    delete render_buffer;
    delete render_target;
}

void SceneFramebuffer::OnUpdate(double delta_time)
{
    Renderer::Get().SetBackgroundColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
}

void SceneFramebuffer::OnRender()
{
    // first pass
    framebuffer->Bind();
    Renderer::Get().Clear();
    Renderer::Get().SetDepthTest(true);

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

        for (auto material : obj->GetAllMaterials()) {
            material->SetUniform("mvp", MVP);
        }

        Renderer::Get().Draw(*obj);
    }

    // second pass
    framebuffer->Unbind();
    Renderer::Get().SetDepthTest(false);
    Renderer::Get().SetBackgroundColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    Renderer::Get().Clear(GL_COLOR_BUFFER_BIT);

    Renderer::Get().Draw(*quad_normal);
}

void SceneFramebuffer::OnImGuiRender()
{

}

void SceneFramebuffer::ConstructScene()
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

REGISTER_SCENE(SceneFramebuffer);