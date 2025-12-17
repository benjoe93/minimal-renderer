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

#include "18_SceneFrameBuffers.h"

static std::vector<float> quad_verts = {
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f, // top left
    -1.0f, -1.0f,  0.0f, 0.0f, // bot left
     1.0f, -1.0f,  1.0f, 0.0f, // bot right
     1.0f,  1.0f,  1.0f, 1.0f  // top right
};
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

namespace scene {
SceneFramebuffer::SceneFramebuffer()
    :Scene("Frame Buffers")
{
    // framebuffer configuration
    framebuffer = std::make_unique<Framebuffer>();

    // create a color attachment texture
    render_target = new RenderTarget(Renderer::Get().state.scr_width, Renderer::Get().state.scr_height, 3);
    framebuffer->AttachRenderTarget(AttachmentTarget::COLOR0, render_target);

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    render_buffer = new RenderBuffer(Renderer::Get().state.scr_width, Renderer::Get().state.scr_height);
    framebuffer->AttachRenderBuffer(AttachmentTarget::DEPTH_STENCIL, render_buffer);
    
    // unbind to prevent accidental renders
    framebuffer->Unbind();

    quad_normal = std::make_unique<Model>(
        std::make_unique<Mesh>(
            verts,
            indices,
            std::make_unique<Material>("resources/shaders/03_AdvancedOpenGL/04_Framebuffer/framebuffer.vert", "resources/shaders/03_AdvancedOpenGL/04_Framebuffer/framebuffer.frag")
        ),
        Transform()
    );

    for (auto& m : quad_normal->GetMeshes())
        m->GetMaterial().AddTexture("screenTexture", render_target);

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

    Camera& cam = Renderer::Get().GetActiveCamera();

    glm::mat4 projection, ModelView, MVP;
    projection = glm::perspective(glm::radians(cam.GetFov()), static_cast<float>(Renderer::Get().state.scr_width) / static_cast<float>(Renderer::Get().state.scr_height), Renderer::Get().state.near_plane, Renderer::Get().state.far_plane);

    // objects
    for (auto& obj : objects)
    {
        ModelView = cam.GetViewMatrix() * obj->GetModelMatrix();
        MVP = projection * ModelView;

        for (auto& mesh : obj->GetMeshes())
            mesh->GetMaterial().SetUniform("mvp", MVP);
    }
}

void SceneFramebuffer::OnRender()
{
    // first pass
    framebuffer->Bind();
    Renderer::Get().Clear();
    Renderer::Get().SetDepthTest(true);

    for (auto& obj : objects)
        Renderer::Get().Draw(*obj);
    
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

void scene::SceneFramebuffer::ConstructScene()
    {
    Texture2D* metal_tex = ResourceManager::Get().GetTexture2D("resources/textures/metal.png", true);
    Texture2D* marble_tex = ResourceManager::Get().GetTexture2D("resources/textures/container.jpg", true);

        std::unique_ptr<Model> floor = std::make_unique<Model>(
            "resources/models/plane.fbx",
            "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.vert",
            "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.frag",
            Transform(
                glm::vec3(0.0f, -0.5f, 0.0f),
                glm::vec3(-90.0f, 0.0f, 0.0f),
                glm::vec3(1.0f)
            )
        );

        for (auto& mesh : floor->GetMeshes())
            mesh->GetMaterial().AddTexture("material.diffuse", metal_tex);
        objects.push_back(std::move(floor));

        // Box 1
        std::unique_ptr<Model> box1 = std::make_unique<Model>(
            "resources/models/box.fbx",
            "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.vert",
            "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.frag",
            Transform(
                glm::vec3(-1.5f, 0.0f, -1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f)
            )
        );

        for (auto& mesh : box1->GetMeshes())
            mesh->GetMaterial().AddTexture("material.diffuse", marble_tex);
        objects.push_back(std::move(box1));

        // Box 2
        std::unique_ptr<Model> box2 = std::make_unique<Model>(
            "resources/models/box.fbx",
            "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.vert",
            "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.frag",
            Transform(
                glm::vec3(1.5f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f)
            )
        );

        for (auto& mesh : box2->GetMeshes())
            mesh->GetMaterial().AddTexture("material.diffuse", marble_tex);
        objects.push_back(std::move(box2));
    }
}