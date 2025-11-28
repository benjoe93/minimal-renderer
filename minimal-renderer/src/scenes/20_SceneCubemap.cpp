#include <map>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vendor/stb_image/stb_image.h>

#include "LightDirectional.h"
#include "LightPoint.h"

#include "Renderer.h"
#include "Camera.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "TextureCubemap.h"

#include "20_SceneCubemap.h"

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

namespace scene {
    SceneCubemap::SceneCubemap(Renderer& in_renderer)
        :Scene(in_renderer, "Cubemap")
    {
        skybox_va = std::make_unique<VertexArray>();
        skybox_va->Bind();

        skybox_vb = std::make_unique<VertexBuffer>(skybox_verts.data(), skybox_verts.size() * sizeof(float));
        skybox_vb->Bind();

        skybox_va->SetLayout(*skybox_vb.get(), 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        cubemap = std::make_shared<TextureCubemap>(2048, 2048, 3, "cubemap", textures_faces);

        skybox_material = std::make_unique<Material>("resources/shaders/03_AdvancedOpenGL/05_Cubemap/cubemap.vert", "resources/shaders/03_AdvancedOpenGL/05_Cubemap/cubemap.frag");
        skybox_material->AddTexture(cubemap);

        ConstructScene();
    }

    void SceneCubemap::OnUpdate(double delta_time)
    {
        m_renderer.SetBackgroundColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

        Camera& cam = m_renderer.GetActiveCamera();

        glm::mat4 projection, ModelView, MVP;
        projection = glm::perspective(glm::radians(cam.GetFov()), static_cast<float>(m_renderer.state.scr_width) / static_cast<float>(m_renderer.state.scr_height), m_renderer.state.near_plane, m_renderer.state.far_plane);

        // objects
        for (auto& obj : objects)
        {
            ModelView = cam.GetViewMatrix() * obj->GetModelMatrix();
            MVP = projection * ModelView;

            for (auto& mesh : obj->GetMeshes())
            {
                mesh->GetMaterial().SetUniform("mvp", MVP);
            }
        }

        // skybox
        glm::mat4 view = glm::mat4(glm::mat3(cam.GetViewMatrix())); // removes translation
        MVP = projection * view;
        skybox_material->SetUniform("mvp", MVP);
    }

    void SceneCubemap::OnRender()
    {

        for (auto& obj : objects)
            m_renderer.Draw(*obj);

        // Optimization: Rendering skybox after the scene is done so fragments covered by the scene doesn't need to be re-rendered.
        m_renderer.SetDepthFunction(TestingFunc::LEQUAL);
        m_renderer.Draw(*skybox_va.get(), static_cast<int>(skybox_verts.size()), *skybox_material.get());
        m_renderer.SetDepthFunction(TestingFunc::LESS);
    }

    void SceneCubemap::OnImGuiRender()
    {

    }

    void SceneCubemap::ConstructScene()
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
