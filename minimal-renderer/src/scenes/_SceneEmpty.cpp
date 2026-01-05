#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "vendor/imgui/imgui.h"

#include "Renderer.h"
#include "Camera.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "ResourceManager.h"

#include "_SceneEmpty.h"

namespace scene {
    _SceneEmpty::_SceneEmpty()
        :Scene("")
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
        Renderer::Get().SetBackgroundColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

        Camera& cam = Renderer::Get().GetActiveCamera();

        glm::mat4 projection, ModelView, MVP;
        projection = glm::perspective(
            glm::radians(cam.GetFov()),
            static_cast<float>(Renderer::Get().GetScreenWidth()) / static_cast<float>(Renderer::Get().GetScreenHeight()),
                Renderer::Get().GetState().near_plane,
                Renderer::Get().GetState().far_plane);

        // objects
        for (auto& obj : m_objects)
        {
            ModelView = cam.GetViewMatrix() * obj->GetModelMatrix();
            MVP = projection * ModelView;

            for (auto& mesh : obj->GetMeshes())
            {
                mesh->GetMaterial().SetUniform("mvp", MVP);
            }
        }
    }

    void _SceneEmpty::OnRender()
    {
        for (auto& obj : m_objects)
            Renderer::Get().Draw(*obj);
    }

    void _SceneEmpty::OnImGuiRender() { }

    void scene::_SceneEmpty::ConstructScene()
    {
        auto floor = new Model(
            "resources/models/plane.fbx",
            "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.vert",
            "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.frag",
            Transform(
                glm::vec3(0.0f, -0.5f, 0.0f),
                glm::vec3(-90.0f, 0.0f, 0.0f),
                glm::vec3(1.0f)
            )
        );

        for (auto& mat : floor->GetMaterials())
            mat->AddTexture("material.diffuse", ResourceManager::Get().GetTexture2D("resources/textures/metal.png"));
        m_objects.push_back(std::move(floor));
        Texture2D* marble_tex = ResourceManager::Get().GetTexture2D("resources/textures/container.jpg");

        // Box 1
        auto box1 = new Model(
            "resources/models/box.fbx",
            "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.vert",
            "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.frag",
            Transform(
                glm::vec3(-1.5f, 0.0f, -1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f)
            )
        );

        for (auto& mat : box1->GetMaterials())
            mat->AddTexture("material.diffuse", marble_tex);
        m_objects.push_back(std::move(box1));

        // Box 2
        auto box2 = new Model(
            "resources/models/box.fbx",
            "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.vert",
            "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.frag",
            Transform(
                glm::vec3(1.5f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f)
            )
        );

        for (auto& mat : box2->GetMaterials())
            mat->AddTexture("material.diffuse", marble_tex);
        m_objects.push_back(std::move(box2));
    }
}
