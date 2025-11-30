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
#include "Texture2D.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"

#include "16_SceneBlending.h"

namespace scene {

    SceneBlending::SceneBlending()
        :Scene("Blending")
    {
        Renderer::Get().SetFaceCulling(false);
        
        window_loc.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
        window_loc.push_back(glm::vec3( 1.5f, 0.0f,  0.51f));
        window_loc.push_back(glm::vec3( 0.0f, 0.0f,  0.7f));
        window_loc.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
        window_loc.push_back(glm::vec3( 0.5f, 0.0f, -0.6f));

        vegetation_loc.push_back(glm::vec3( 0.5f, -0.25f,  0.48f));
        vegetation_loc.push_back(glm::vec3(-2.5f, -0.25f, -0.51f));
        vegetation_loc.push_back(glm::vec3( 0.0f, -0.25f,  0.2f));

        ////////////////////////////////////////////////////////////////////////////
        //                            geometery setup                             //
        ////////////////////////////////////////////////////////////////////////////
        // floor
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
                mesh->GetMaterial().AddTexture2D("resources/textures/metal.png", "material.diffuse", true);
            objects.push_back(std::move(floor));
        }

        std::shared_ptr<Texture2D> marble_tex = std::make_shared<Texture2D>("resources/textures/marble.jpg", "material.diffuse", true);
        
        // Box 1
        {
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

        }
        // Box 2
        {
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

        // windows
        std::shared_ptr<Texture2D> window_tex = std::make_shared<Texture2D>("resources/textures/window.png", "material.diffuse", true);
        for (unsigned int i = 0; i < window_loc.size(); i++)
        {
            std::unique_ptr<Model> window = std::make_unique<Model>(
                "resources/models/plane.fbx",
                "resources/shaders/03_AdvancedOpenGL/03_Blending/window.vert",
                "resources/shaders/03_AdvancedOpenGL/03_Blending/window.frag",
                Transform(
                    window_loc[i], 
                    glm::vec3(0.0f, 0.0f, 180.0f),
                    glm::vec3(0.1f)
                )
            );

            for (auto& mesh : window->GetMeshes())
                mesh->GetMaterial().AddTexture(window_tex);
            transparent_objects.push_back(std::move(window));
        }

        // grass
        std::shared_ptr<Texture2D> grass_tex = std::make_shared<Texture2D>("resources/textures/grass.png", "material.diffuse", true);
        grass_tex->SetWrappingHorizontal(GL_CLAMP_TO_EDGE);
        grass_tex->SetWrappingVertical(GL_CLAMP_TO_EDGE);
        for (unsigned int i = 0; i < vegetation_loc.size(); i++)
        {
            std::unique_ptr<Model> grass = std::make_unique<Model>(
                "resources/models/plane.fbx",
                "resources/shaders/03_AdvancedOpenGL/03_Blending/grass.vert",
                "resources/shaders/03_AdvancedOpenGL/03_Blending/grass.frag",
                Transform(
                    vegetation_loc[i],
                    glm::vec3(0.0f, 0.0f, 180.0f),
                    glm::vec3(0.05f)
                )
            );

            for (auto& mesh : grass->GetMeshes())
                mesh->GetMaterial().AddTexture(grass_tex);
            objects.push_back(std::move(grass));
        }
    }

    void SceneBlending::OnUpdate(double delta_time)
    {
        Renderer::Get().SetBackgroundColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

        Camera& cam = Renderer::Get().GetActiveCamera();

        glm::mat4 projection, ModelView, MVP;
        projection = glm::perspective(glm::radians(cam.GetFov()), static_cast<float>(Renderer::Get().state.scr_width) / static_cast<float>(Renderer::Get().state.scr_height), Renderer::Get().state.near_plane, Renderer::Get().state.far_plane);

        ////////////////////////////////////////////////////////////////////////////
        //                           geometery update                             //
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
        for (unsigned int i = 0; i < transparent_objects.size(); i++)
        {
            float distance = glm::length(Renderer::Get().GetActiveCamera().GetPosition() - transparent_objects[i]->GetLocation());
            sorted[distance] = transparent_objects[i].get();
        }

        for (auto& obj : objects)
            Renderer::Get().Draw(*obj);

        // render object in reverse distance order
        for (std::map<float, Model*>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
            Renderer::Get().Draw(*it->second);
    }

    void SceneBlending::OnImGuiRender()
    {
    }
}