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

#include "09_SceneDepthTesting.h"



namespace scene {

    SceneDepthTesting::SceneDepthTesting(Renderer& in_renderer)
        :Scene(in_renderer)
    {
        m_renderer.SetBackgroundColor(glm::vec4(0.18f, 0.23f, 0.24f, 1.0f));

        Camera& camera = m_renderer.GetActiveCamera();

        // object setup
        objects.push_back(std::make_unique<Model>("resources/models/box.fbx",   "resources/shaders/03_AdvancedOpenGL/01_DepthTesting/depth_test.vert", "resources/shaders/03_AdvancedOpenGL/01_DepthTesting/depth_test.frag"));
        objects.push_back(std::make_unique<Model>("resources/models/box.fbx",   "resources/shaders/03_AdvancedOpenGL/01_DepthTesting/depth_test.vert", "resources/shaders/03_AdvancedOpenGL/01_DepthTesting/depth_test.frag"));
        objects.push_back(std::make_unique<Model>("resources/models/plane.fbx", "resources/shaders/03_AdvancedOpenGL/01_DepthTesting/depth_test.vert", "resources/shaders/03_AdvancedOpenGL/01_DepthTesting/depth_test.frag"));
        for (auto& mesh : objects[0]->GetMeshes())
            mesh->GetMaterial().AddTexture("resources/textures/marble.jpg", TextureType::DIFFUSE, true);
        for (auto& mesh : objects[1]->GetMeshes())
            mesh->GetMaterial().AddTexture("resources/textures/marble.jpg", TextureType::DIFFUSE, true);
        for (auto& mesh : objects[2]->GetMeshes())
            mesh->GetMaterial().AddTexture("resources/textures/metal.png", TextureType::DIFFUSE, true);
    }

    void SceneDepthTesting::OnUpdate(double delta_time)
    {
        Camera& cam = m_renderer.GetActiveCamera();
        glm::vec3 cam_pos = cam.GetPosition();

        glm::mat4 projection, model, ModelView, MVP;
        projection = glm::perspective(glm::radians(cam.GetFov()), static_cast<float>(m_renderer.state->scr_width) / static_cast<float>(m_renderer.state->scr_height), 0.1f, 100.0f);

        // objects
        // boxes
        for (auto& m : objects[0]->GetMeshes())
        {
            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
            //model = glm::scale(model, glm::vec3(0.2f));
            ModelView = cam.GetViewMatrix() * model;
            MVP = projection * ModelView;

            auto& material = m->GetMaterial();
            material.SetUniformMat4("model", model);
            material.SetUniformMat4("mvp", MVP);
        }

        for (auto& m : objects[1]->GetMeshes())
        {
            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
            //model = glm::scale(model, glm::vec3(0.2f));
            ModelView = cam.GetViewMatrix() * model;
            MVP = projection * ModelView;

            auto& material = m->GetMaterial();
            material.SetUniformMat4("model", model);
            material.SetUniformMat4("mvp", MVP);
        }

        // plane
        for (auto& m : objects[2]->GetMeshes())
        {
            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            ModelView = cam.GetViewMatrix() * model;
            MVP = projection * ModelView;

            auto& material = m->GetMaterial();
            material.SetUniformMat4("model", model);
            material.SetUniformMat4("mvp", MVP);
        }
    }

    void SceneDepthTesting::OnRender()
    {
        m_renderer.SetBackgroundColor(glm::vec4(0.18f, 0.23f, 0.24f, 1.0f));

        // object rendering
        for (auto& obj : objects)
        {
            m_renderer.Draw(*obj);
        }
    }

    void SceneDepthTesting::OnImGuiRender()
    {
    }
}