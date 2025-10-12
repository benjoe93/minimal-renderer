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

#include "12_SceneFaceCulling.h"

namespace scene {

    SceneFaceCulling::SceneFaceCulling(Renderer& in_renderer)
        :Scene(in_renderer)
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

        std::shared_ptr<Texture> marble_tex = std::make_shared<Texture>("resources/textures/marble.jpg", TextureType::DIFFUSE, true);

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

    void SceneFaceCulling::OnUpdate(double delta_time)
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

    void SceneFaceCulling::OnRender()
    {
        for (auto& obj : objects)
            m_renderer.Draw(*obj);
    }

    void SceneFaceCulling::OnImGuiRender()
    {
        
    }
}