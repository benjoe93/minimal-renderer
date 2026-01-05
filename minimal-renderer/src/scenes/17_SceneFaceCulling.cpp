#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "vendor/imgui/imgui.h"

#include "Renderer.h"
#include "Camera.h"
#include "Texture2D.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "ResourceManager.h"

#include "17_SceneFaceCulling.h"

namespace scene {

    SceneFaceCulling::SceneFaceCulling()
        :Scene("Face Culling")
    {
        Texture2D* metal_tex = ResourceManager::Get().GetTexture2D("resources/textures/metal.png", true);
        Texture2D* marble_tex = ResourceManager::Get().GetTexture2D("resources/textures/container.jpg", true);
        std::string vertex_path = "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.vert";
        std::string fragment_path = "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.frag";

        auto floor = std::make_unique<Model>(
            "resources/models/plane.fbx",
            vertex_path,
            fragment_path,
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
        auto box1 = std::make_unique<Model>(
            "resources/models/box.fbx",
            vertex_path,
            fragment_path,
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
            vertex_path,
            fragment_path,
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

    void SceneFaceCulling::OnUpdate(double delta_time)
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
        for (auto& obj : objects)
        {
            ModelView = cam.GetViewMatrix() * obj->GetModelMatrix();
            MVP = projection * ModelView;

            for (auto& mesh : obj->GetMeshes())
                mesh->GetMaterial().SetUniform("mvp", MVP);
        }
    }

    void SceneFaceCulling::OnRender()
    {
        for (auto& obj : objects)
            Renderer::Get().Draw(*obj);
    }

    void SceneFaceCulling::OnImGuiRender() { }
}