#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "vendor/imgui/imgui.h"

#include "LightDirectional.h"
#include "LightPoint.h"

#include "Renderer.h"
#include "Camera.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"

#include "13_SceneFrameBuffers.h"

static std::vector<float> quad_verts = {
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f, // top left
    -1.0f, -1.0f,  0.0f, 0.0f, // bot left
     1.0f, -1.0f,  1.0f, 0.0f, // bot right
     1.0f,  1.0f,  1.0f, 1.0f  // top right
};

static std::vector<unsigned int> indices = 
{
    0, 1, 2,
    0, 2, 3
};

namespace scene {
SceneFramebuffer::SceneFramebuffer(Renderer& in_renderer)
    :Scene(in_renderer)
{
    // framebuffer configuration
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // create a color attachment texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_renderer.state->scr_width, m_renderer.state->scr_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_renderer.state->scr_width, m_renderer.state->scr_height); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    
    // unbind to prevent accidental renders
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Create quad
    va = std::make_unique<VertexArray>();
    va->Bind();

    vb = std::make_unique<VertexBuffer>(quad_verts.data(), quad_verts.size() * sizeof(float));
    vb->Bind();

    ib = std::make_unique<IndexBuffer>(indices.data(), indices.size());
    ib->Bind();

    va->SetLayout(*vb, 0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    va->SetLayout(*vb, 1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    mat = std::make_unique<Material>("resources/shaders/03_AdvancedOpenGL/04_Framebuffer/framebuffer.vert", "resources/shaders/03_AdvancedOpenGL/04_Framebuffer/framebuffer.frag");

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
        {
            mesh->GetMaterial().SetUniformMat4("mvp", MVP);
        }
    }

}

void SceneFramebuffer::OnRender()
{
    // first pass
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    m_renderer.Clear();
    m_renderer.SetDepthTest(true);

    for (auto& obj : objects)
        m_renderer.Draw(*obj);
    
    // second pass
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    m_renderer.SetDepthTest(false);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    mat->Bind();
    mat->GetShader().Bind();
    mat->GetShader().SetInt("screenTexture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    m_renderer.Draw(*va, *ib, mat->GetShader());
    mat->Unbind();
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
            mesh->GetMaterial().AddTexture("resources/textures/metal.png", TextureType::DIFFUSE, true);
        objects.push_back(std::move(floor));
        std::shared_ptr<Texture> marble_tex = std::make_shared<Texture>("resources/textures/container.jpg", TextureType::DIFFUSE, true);

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
            mesh->GetMaterial().AddTexture(marble_tex, TextureType::DIFFUSE);
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
            mesh->GetMaterial().AddTexture(marble_tex, TextureType::DIFFUSE);
        objects.push_back(std::move(box2));
    }
}