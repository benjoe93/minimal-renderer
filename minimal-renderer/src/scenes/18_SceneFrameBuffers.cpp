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
SceneFramebuffer::SceneFramebuffer(Renderer& in_renderer)
    :Scene(in_renderer, "Frame Buffers")
{
    // framebuffer configuration
    framebuffer = std::make_unique<Framebuffer>();

    // create a color attachment texture
    render_target = std::make_shared<RenderTarget>(m_renderer.state->scr_width, m_renderer.state->scr_height, 3, "screenTexture");
    framebuffer->AttachRenderTarget(AttachmentTarget::COLOR0, render_target);

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    render_buffer = std::make_shared<RenderBuffer>(m_renderer.state->scr_width, m_renderer.state->scr_height);
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
        m->GetMaterial().AddTexture(render_target);

    ConstructScene();
}

void SceneFramebuffer::OnUpdate(double delta_time)
{
    m_renderer.SetBackgroundColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

    Camera& cam = m_renderer.GetActiveCamera();

    glm::mat4 projection, ModelView, MVP;
    projection = glm::perspective(glm::radians(cam.GetFov()), static_cast<float>(m_renderer.state->scr_width) / static_cast<float>(m_renderer.state->scr_height), m_renderer.state->near_plane, m_renderer.state->far_plane);

    // objects
    for (auto& obj : objects)
    {
        ModelView = cam.GetViewMatrix() * obj->GetModelMatrix();
        MVP = projection * ModelView;

        for (auto& mesh : obj->GetMeshes())
            mesh->GetMaterial().SetUniformMat4("mvp", MVP);
    }
}

void SceneFramebuffer::OnRender()
{
    // first pass
    framebuffer->Bind();
    m_renderer.Clear();
    m_renderer.SetDepthTest(true);

    for (auto& obj : objects)
        m_renderer.Draw(*obj);
    
    // second pass
    framebuffer->Unbind();
    m_renderer.SetDepthTest(false);
    m_renderer.SetBackgroundColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    m_renderer.Clear(GL_COLOR_BUFFER_BIT);

    m_renderer.Draw(*quad);
}

void SceneFramebuffer::OnImGuiRender()
{
    
}

void scene::SceneFramebuffer::ConstructScene()
    {
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
            mesh->GetMaterial().AddTexture(std::make_shared<Texture2D>("resources/textures/metal.png", "material.diffuse", true));
        objects.push_back(std::move(floor));
        std::shared_ptr<Texture2D> marble_tex = std::make_shared<Texture2D>("resources/textures/container.jpg", "material.diffuse", true);

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
            mesh->GetMaterial().AddTexture(marble_tex);
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
            mesh->GetMaterial().AddTexture(marble_tex);
        objects.push_back(std::move(box2));
    }
}