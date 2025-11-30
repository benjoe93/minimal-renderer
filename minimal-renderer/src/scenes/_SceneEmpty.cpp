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

#include "20_SceneCubeMap.h"

namespace scene {
    SceneCubeMap::SceneCubeMap(Renderer& in_renderer)
        :Scene(in_renderer, "Cube Map")
    {
        ConstructScene();
    }

    void SceneCubeMap::OnUpdate(double delta_time)
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
            {
                mesh->GetMaterial().SetUniform("mvp", MVP);
            }
        }
    }

    void SceneCubeMap::OnRender()
    {
        for (auto& obj : objects)
            Renderer::Get().Draw(*obj);
    }

    void SceneCubeMap::OnImGuiRender()
    {

    }

    void scene::SceneCubeMap::ConstructScene()
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
