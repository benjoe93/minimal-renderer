#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "Camera.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "ResourceManager.h"

#include "14_SceneDepthTesting.h"

namespace scene {

    SceneDepthTesting::SceneDepthTesting()
        :Scene("Depth Testing")
    {
        Renderer::Get().SetBackgroundColor(glm::vec4(0.18f, 0.23f, 0.24f, 1.0f));

        Camera& camera = Renderer::Get().GetActiveCamera();

        ////////////////////////////////////////////////////////////////////////////
        //                            geometry setup                              //
        ////////////////////////////////////////////////////////////////////////////
        objects.push_back(std::make_unique<Model>("resources/models/box.fbx"));
        objects.push_back(std::make_unique<Model>("resources/models/box.fbx"));
        objects.push_back(std::make_unique<Model>("resources/models/plane.fbx"));

        Material* material = ResourceManager::Get().GetMaterial(
            "resources/shaders/03_AdvancedOpenGL/01_DepthTesting/depth_test.vert",
            "resources/shaders/03_AdvancedOpenGL/01_DepthTesting/depth_test.frag"
        );

        Texture2D* marble_tex = ResourceManager::Get().GetTexture2D("resources/textures/marble.jpg", true);
        Texture2D* metal_tex = ResourceManager::Get().GetTexture2D("resources/textures/metal.png", true);

        material->AddTexture("material.diffuse", marble_tex);

        // Set materials for boxes
        for (int i = 0; i < 2; i++) {
            objects[i]->SetMaterialSlot(0, material);
        }

        // Floor gets different texture
        objects[2]->SetMaterialSlot(0, material);
        for (auto& mesh : objects[2]->GetMeshes()) {
            Material* floor_mat = objects[2]->GetMaterialForMesh(mesh.get());
            if (floor_mat) {
                floor_mat->AddTexture("material.diffuse", metal_tex);
            }
        }
    }

    void SceneDepthTesting::OnUpdate(double delta_time)
    {
        Camera& cam = Renderer::Get().GetActiveCamera();
        glm::vec3 cam_pos = cam.GetPosition();

        glm::mat4 projection, model, ModelView, MVP;
        projection = glm::perspective(glm::radians(cam.GetFov()), static_cast<float>(Renderer::Get().GetScreenWidth()) / static_cast<float>(Renderer::Get().GetScreenHeight()), 0.1f, 100.0f);

        ////////////////////////////////////////////////////////////////////////////
        //                           geometry update                              //
        ////////////////////////////////////////////////////////////////////////////
        // boxes
        for (auto& m : objects[0]->GetMeshes())
        {
            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
            ModelView = cam.GetViewMatrix() * model;
            MVP = projection * ModelView;

            Material* material = objects[0]->GetMaterialForMesh(m.get());
            if (material) {
                material->SetUniform("model", model);
                material->SetUniform("mvp", MVP);
            }
        }

        for (auto& m : objects[1]->GetMeshes())
        {
            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
            ModelView = cam.GetViewMatrix() * model;
            MVP = projection * ModelView;

            Material* material = objects[1]->GetMaterialForMesh(m.get());
            if (material) {
                material->SetUniform("model", model);
                material->SetUniform("mvp", MVP);
            }
        }

        // plane
        for (auto& m : objects[2]->GetMeshes())
        {
            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            ModelView = cam.GetViewMatrix() * model;
            MVP = projection * ModelView;

            Material* material = objects[2]->GetMaterialForMesh(m.get());
            if (material) {
                material->SetUniform("model", model);
                material->SetUniform("mvp", MVP);
            }
        }
    }

    void SceneDepthTesting::OnRender()
    {
        Renderer::Get().SetBackgroundColor(glm::vec4(0.18f, 0.23f, 0.24f, 1.0f));

        ////////////////////////////////////////////////////////////////////////////
        //                          geometry rendering                            //
        ////////////////////////////////////////////////////////////////////////////
        for (auto& obj : objects)
            Renderer::Get().Draw(*obj);
    }

    void SceneDepthTesting::OnImGuiRender() { }
}