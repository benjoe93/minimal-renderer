#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "Camera.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "ResourceManager.h"

#include "15_SceneStencilTesting.h"

namespace scene {

    SceneStencilTesting::SceneStencilTesting()
        :Scene("Stencil Testing")
    {
        Renderer::Get().SetBackgroundColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

        Renderer::Get().SetDepthTest(true);
        Renderer::Get().SetDepthFunction(TestingFunc::LESS);
        Renderer::Get().SetStencilTest(true);
        Renderer::Get().SetStencilFunction(TestingFunc::NOTEQUAL, 1, 0xFF);
        Renderer::Get().SetStencilOperation(StencilOp::KEEP, StencilOp::KEEP, StencilOp::REPLACE);

        Camera& camera = Renderer::Get().GetActiveCamera();

        Texture2D* metal_tex = ResourceManager::Get().GetTexture2D("resources/textures/metal.png", true);
        Texture2D* marble_tex = ResourceManager::Get().GetTexture2D("resources/textures/marble.jpg", true);

        // object setup
        objects.push_back(std::make_unique<Model>("resources/models/plane.fbx"));
        objects.push_back(std::make_unique<Model>("resources/models/box.fbx"));
        objects.push_back(std::make_unique<Model>("resources/models/box.fbx"));

        for (auto& mesh : objects[0]->GetMeshes()) {
            Material* mat = objects[0]->GetMaterialForMesh(mesh.get());
            if (mat) mat->AddTexture("material.diffuse", metal_tex);
        }
        for (auto& mesh : objects[1]->GetMeshes()) {
            Material* mat = objects[1]->GetMaterialForMesh(mesh.get());
            if (mat) mat->AddTexture("material.diffuse", marble_tex);
        }
        for (auto& mesh : objects[2]->GetMeshes()) {
            Material* mat = objects[2]->GetMaterialForMesh(mesh.get());
            if (mat) mat->AddTexture("material.diffuse", marble_tex);
        }

        // boxes for the outline
        outline_objects.push_back(std::make_unique<Model>("resources/models/box.fbx"));
        outline_objects.push_back(std::make_unique<Model>("resources/models/box.fbx"));

        Material* outline_mat = ResourceManager::Get().GetMaterial(
            "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/stencil_test.vert",
            "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/stencil_test.frag"
        );

        for (auto& obj : outline_objects) {
            obj->SetMaterialSlot(0, outline_mat);
        }
    }

    void SceneStencilTesting::OnUpdate(double delta_time)
    {
        Renderer::Get().SetBackgroundColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

        Camera& cam = Renderer::Get().GetActiveCamera();
        glm::vec3 cam_pos = cam.GetPosition();

        glm::mat4 projection, model, ModelView, MVP;
        projection = glm::perspective(glm::radians(cam.GetFov()), static_cast<float>(Renderer::Get().GetScreenWidth()) / static_cast<float>(Renderer::Get().GetScreenHeight()), 0.1f, 100.0f);

        // objects
        // plane
        for (auto& m : objects[0]->GetMeshes())
        {
            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
            model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            ModelView = cam.GetViewMatrix() * model;
            MVP = projection * ModelView;

            Material* material = objects[0]->GetMaterialForMesh(m.get());
            if (material) {
                material->SetUniform("model", model);
                material->SetUniform("mvp", MVP);
            }
        }

        // boxes for rendering
        for (auto& m : objects[1]->GetMeshes())
        {
            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
            ModelView = cam.GetViewMatrix() * model;
            MVP = projection * ModelView;

            Material* material = objects[1]->GetMaterialForMesh(m.get());
            if (material) {
                material->SetUniform("model", model);
                material->SetUniform("mvp", MVP);
            }
        }

        for (auto& m : objects[2]->GetMeshes())
        {
            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
            ModelView = cam.GetViewMatrix() * model;
            MVP = projection * ModelView;

            Material* material = objects[2]->GetMaterialForMesh(m.get());
            if (material) {
                material->SetUniform("model", model);
                material->SetUniform("mvp", MVP);
            }
        }

        // boxes for outline
        for (auto& m : outline_objects[0]->GetMeshes())
        {
            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
            model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
            ModelView = cam.GetViewMatrix() * model;
            MVP = projection * ModelView;

            Material* material = outline_objects[0]->GetMaterialForMesh(m.get());
            if (material) {
                material->SetUniform("model", model);
                material->SetUniform("mvp", MVP);
            }
        }

        for (auto& m : outline_objects[1]->GetMeshes())
        {
            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
            ModelView = cam.GetViewMatrix() * model;
            MVP = projection * ModelView;

            Material* material = outline_objects[1]->GetMaterialForMesh(m.get());
            if (material) {
                material->SetUniform("model", model);
                material->SetUniform("mvp", MVP);
            }
        }
    }

    void SceneStencilTesting::OnRender()
    {
        Renderer::Get().SetDepthTest(true);
        Renderer::Get().SetStencilOperation(StencilOp::KEEP, StencilOp::KEEP, StencilOp::REPLACE);

        Renderer::Get().SetStencilMask(0x00);
        Renderer::Get().Draw(*objects[0]);

        Renderer::Get().SetStencilFunction(TestingFunc::ALWAYS, 1, 0xFF);
        Renderer::Get().SetStencilMask(0xFF);
        for (unsigned int i = 1; i < 3; i++)
        {
            Renderer::Get().Draw(*objects[i]);
        }

        Renderer::Get().SetStencilFunction(TestingFunc::NOTEQUAL, 1, 0xFF);
        Renderer::Get().SetStencilMask(0x00);
        Renderer::Get().SetDepthTest(false);
        for (auto& obj : outline_objects)
        {
            Renderer::Get().Draw(*obj);
        }
        Renderer::Get().SetStencilMask(0xFF);
        Renderer::Get().SetStencilFunction(TestingFunc::ALWAYS, 0, 0xFF);
        Renderer::Get().SetDepthTest(true);
    }

    void SceneStencilTesting::OnImGuiRender() { }
}