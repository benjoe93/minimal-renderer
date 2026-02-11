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

        Material* object_material = ResourceManager::Get().GetMaterial(
            "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.vert",
            "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.frag"
        );

        auto floor = std::make_unique<Model>(
            "resources/models/plane.fbx",
            Transform(glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(-90.0f, 0.0f, 0.0f), glm::vec3(1.0f))
        );
        auto box1 = std::make_unique<Model>(
            "resources/models/box.fbx",
            Transform(glm::vec3(-1.5f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f))
        );
        auto box2 = std::make_unique<Model>(
            "resources/models/box.fbx",
            Transform(glm::vec3(1.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f))
        );

        floor->SetMaterialSlot(0, object_material);
        box1->SetMaterialSlot(0, object_material);
        box2->SetMaterialSlot(0, object_material);

        for (auto& mesh : floor->GetMeshes()) {
            Material* mat = floor->GetMaterialForMesh(mesh.get());
            if (mat) mat->AddTexture("material.diffuse", metal_tex);
        }
        for (auto& mesh : box1->GetMeshes()) {
            Material* mat = box1->GetMaterialForMesh(mesh.get());
            if (mat) mat->AddTexture("material.diffuse", marble_tex);
        }
        for (auto& mesh : box2->GetMeshes()) {
            Material* mat = box2->GetMaterialForMesh(mesh.get());
            if (mat) mat->AddTexture("material.diffuse", marble_tex);
        }

        objects.push_back(std::move(floor));
        objects.push_back(std::move(box1));
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

            for (auto& mesh : obj->GetMeshes()) {
                Material* material = obj->GetMaterialForMesh(mesh.get());
                if (material) {
                    material->SetUniform("mvp", MVP);
                }
            }
        }
    }

    void SceneFaceCulling::OnRender()
    {
        for (auto& obj : objects)
            Renderer::Get().Draw(*obj);
    }

    void SceneFaceCulling::OnImGuiRender() { }
}