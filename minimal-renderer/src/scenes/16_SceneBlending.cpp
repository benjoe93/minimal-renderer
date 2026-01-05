#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "Camera.h"
#include "IndexBuffer.h"
#include "Texture2D.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "ResourceManager.h"

#include "16_SceneBlending.h"

namespace scene {

    SceneBlending::SceneBlending()
        :Scene("Blending")
    {
        Renderer::Get().SetFaceCulling(false);
        
        window_loc.emplace_back(-1.5f, 0.0f, -0.48f);
        window_loc.emplace_back( 1.5f, 0.0f,  0.51f);
        window_loc.emplace_back( 0.0f, 0.0f,  0.7f);
        window_loc.emplace_back(-0.3f, 0.0f, -2.3f);
        window_loc.emplace_back( 0.5f, 0.0f, -0.6f);

        vegetation_loc.emplace_back( 0.5f, -0.25f,  0.48f);
        vegetation_loc.emplace_back(-2.5f, -0.25f, -0.51f);
        vegetation_loc.emplace_back( 0.0f, -0.25f,  0.2f);

        ////////////////////////////////////////////////////////////////////////////
        //                            geometry setup                              //
        ////////////////////////////////////////////////////////////////////////////
        // floor
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
        auto box2 = std::make_unique<Model>(
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

        // windows
        Texture2D* window_tex = ResourceManager::Get().GetTexture2D("resources/textures/window.png", true);
        for (auto location : window_loc)
        {
            auto window = std::make_unique<Model>(
                "resources/models/plane.fbx",
                "resources/shaders/03_AdvancedOpenGL/03_Blending/window.vert",
                "resources/shaders/03_AdvancedOpenGL/03_Blending/window.frag",
                Transform(
                    location,
                    glm::vec3(0.0f, 0.0f, 180.0f),
                    glm::vec3(0.1f)
                )
            );

            for (auto& mesh : window->GetMeshes())
                mesh->GetMaterial().AddTexture("material.diffuse", window_tex);
            transparent_objects.push_back(std::move(window));
        }

        // grass
        Texture2D* grass_tex = ResourceManager::Get().GetTexture2D("resources/textures/grass.png", true);
        grass_tex->SetWrappingHorizontal(GL_CLAMP_TO_EDGE);
        grass_tex->SetWrappingVertical(GL_CLAMP_TO_EDGE);
        for (auto location : vegetation_loc)
        {
            auto grass = std::make_unique<Model>(
                "resources/models/plane.fbx",
                "resources/shaders/03_AdvancedOpenGL/03_Blending/grass.vert",
                "resources/shaders/03_AdvancedOpenGL/03_Blending/grass.frag",
                Transform(
                    location,
                    glm::vec3(0.0f, 0.0f, 180.0f),
                    glm::vec3(0.05f)
                )
            );

            for (auto& mesh : grass->GetMeshes())
                mesh->GetMaterial().AddTexture("material.diffuse", grass_tex);
            objects.push_back(std::move(grass));
        }
    }

    void SceneBlending::OnUpdate(double delta_time)
    {
        Renderer::Get().SetBackgroundColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

        Camera& cam = Renderer::Get().GetActiveCamera();

        glm::mat4 projection, ModelView, MVP;
        projection = glm::perspective(
            glm::radians(cam.GetFov()),
            static_cast<float>(Renderer::Get().GetScreenWidth()) / static_cast<float>(Renderer::Get().GetScreenHeight()),
            Renderer::Get().GetState().near_plane,
            Renderer::Get().GetState().far_plane
        );

        ////////////////////////////////////////////////////////////////////////////
        //                           geometry update                             //
        ////////////////////////////////////////////////////////////////////////////
        for (auto& obj : objects)
        {
            ModelView = cam.GetViewMatrix() * obj->GetModelMatrix();
            MVP = projection * ModelView;

            for (auto& mesh : obj->GetMeshes())
            {
                mesh->GetMaterial().SetUniform("mvp", MVP);
            }
        }

        for (auto& obj : transparent_objects)
        {
            ModelView = cam.GetViewMatrix() * obj->GetModelMatrix();
            MVP = projection * ModelView;

            for (auto& mesh : obj->GetMeshes())
            {
                mesh->GetMaterial().SetUniform("mvp", MVP);
            }
        }
    }

    void SceneBlending::OnRender()
    {
        // sort transparent objects based on distance from camera
        std::map<float, Model*> sorted;
        for (const auto & transparent_object : transparent_objects)
        {
            float distance = glm::length(Renderer::Get().GetActiveCamera().GetPosition() - transparent_object->GetLocation());
            sorted[distance] = transparent_object.get();
        }

        for (auto& obj : objects)
            Renderer::Get().Draw(*obj);

        // render object in reverse distance order
        for (auto it = sorted.rbegin(); it != sorted.rend(); ++it)
            Renderer::Get().Draw(*it->second);
    }

    void SceneBlending::OnImGuiRender() { }
}