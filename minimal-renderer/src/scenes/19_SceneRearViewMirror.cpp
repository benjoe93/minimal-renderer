#include <map>
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

#include "19_SceneRearViewMirror.h"

static std::vector<Vertex> verts = {
           // positions          // normals          // texCoords
    Vertex({-0.5f,  0.9f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}), // top left
    Vertex({-0.5f,  0.4f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}), // bot left
    Vertex({ 0.5f,  0.4f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}), // bot right
    Vertex({ 0.5f,  0.9f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}), // top right
};
static std::vector<unsigned int> indices = {
    0, 1, 2,
    0, 2, 3
};

namespace scene {
SceneRearViewMirror::SceneRearViewMirror()
    :Scene("Rear View Mirror")
{
    // framebuffer configuration
    framebuffer = std::make_unique<Framebuffer>();

    // create a color attachment texture
    render_target = new RenderTarget(Renderer::Get().GetScreenWidth(), Renderer::Get().GetScreenHeight(), 3);
    framebuffer->AttachRenderTarget(AttachmentTarget::COLOR0, render_target);

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    render_buffer = new RenderBuffer(Renderer::Get().GetScreenWidth(), Renderer::Get().GetScreenHeight());
    framebuffer->AttachRenderBuffer(AttachmentTarget::DEPTH_STENCIL, render_buffer);
    
    // unbind to prevent accidental renders
    framebuffer->Unbind();

    // create mesh list
    std::vector<std::unique_ptr<Mesh>> meshes;
    meshes.push_back(std::make_unique<Mesh>(verts, indices));

    quad = std::make_unique<Model>(
        std::move(meshes),
        Transform()
    );

    Material* quad_material = ResourceManager::Get().GetMaterial(
        "resources/shaders/03_AdvancedOpenGL/04_Framebuffer/framebuffer.vert",
        "resources/shaders/03_AdvancedOpenGL/04_Framebuffer/mirror.frag"
    );
    quad->SetMaterialSlot(0, quad_material);

    for (auto& m : quad->GetMeshes()) {
        Material* mat = quad->GetMaterialForMesh(m.get());
        if (mat) mat->AddTexture("screenTexture", render_target);
    }

    ConstructScene();
}

SceneRearViewMirror::~SceneRearViewMirror()
{
    delete render_buffer;
    delete render_target;
}

void SceneRearViewMirror::OnUpdate(double delta_time)
{
    Renderer::Get().SetBackgroundColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
}

void SceneRearViewMirror::OnRender()
{
    Camera& cam = Renderer::Get().GetActiveCamera();

    glm::mat4 projection, ModelView, MVP;
    projection = glm::perspective(
        glm::radians(cam.GetFov()),
        static_cast<float>(Renderer::Get().GetScreenWidth()) / static_cast<float>(Renderer::Get().GetScreenHeight()),
        Renderer::Get().GetState().near_plane,
        Renderer::Get().GetState().far_plane);

    // first pass
    framebuffer->Bind();
    Renderer::Get().Clear();
    Renderer::Get().SetDepthTest(true);

    cam.AddYaw(180.0f);
    for (auto& obj : objects)
    {
        ModelView = cam.GetViewMatrix() * obj->GetModelMatrix();
        MVP = projection * ModelView;

        for (auto& mesh : obj->GetMeshes()) {
            Material* material = obj->GetMaterialForMesh(mesh.get());
            if (material) {
                material->SetUniform("mvp", MVP);
            }
        }

        Renderer::Get().Draw(*obj);
    }

    // second pass
    framebuffer->Unbind();
    Renderer::Get().Clear();

    cam.AddYaw(-180.0f);
    for (auto& obj : objects)
    {
        ModelView = cam.GetViewMatrix() * obj->GetModelMatrix();
        MVP = projection * ModelView;

        for (auto& mesh : obj->GetMeshes()) {
            Material* material = obj->GetMaterialForMesh(mesh.get());
            if (material) {
                material->SetUniform("mvp", MVP);
            }
        }
        Renderer::Get().Draw(*obj);
    }

    Renderer::Get().SetDepthTest(false);
    Renderer::Get().Draw(*quad);
}

void SceneRearViewMirror::OnImGuiRender() { }

void scene::SceneRearViewMirror::ConstructScene()
{
    Texture2D* metal_tex = ResourceManager::Get().GetTexture2D("resources/textures/metal.png", true);
    Texture2D* marble_tex = ResourceManager::Get().GetTexture2D("resources/textures/container.jpg", true);

    Material* object_material = ResourceManager::Get().GetMaterial(
        "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.vert",
        "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.frag"
    );

    auto floor = std::make_unique<Model>(
        "resources/models/plane.fbx",
        Transform(
            glm::vec3(0.0f, -0.5f, 0.0f),
            glm::vec3(-90.0f, 0.0f, 0.0f),
            glm::vec3(1.0f)
        )
    );
    floor->SetMaterialSlot(0, object_material);

    for (auto& mesh : floor->GetMeshes()) {
        Material* mat = floor->GetMaterialForMesh(mesh.get());
        if (mat) mat->AddTexture("material.diffuse", metal_tex);
    }
    objects.push_back(std::move(floor));

    // Box 1
    auto box1 = std::make_unique<Model>(
        "resources/models/box.fbx",
        Transform(
            glm::vec3(-1.5f, 0.0f, -1.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f)
        )
    );
    box1->SetMaterialSlot(0, object_material);

    for (auto& mesh : box1->GetMeshes()) {
        Material* mat = box1->GetMaterialForMesh(mesh.get());
        if (mat) mat->AddTexture("material.diffuse", marble_tex);
    }
    objects.push_back(std::move(box1));

    // Box 2
    auto box2 = std::make_unique<Model>(
        "resources/models/box.fbx",
        Transform(
            glm::vec3(1.5f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f)
        )
    );
    box2->SetMaterialSlot(0, object_material);

    for (auto& mesh : box2->GetMeshes()) {
        Material* mat = box2->GetMaterialForMesh(mesh.get());
        if (mat) mat->AddTexture("material.diffuse", marble_tex);
    }
    objects.push_back(std::move(box2));
}
}