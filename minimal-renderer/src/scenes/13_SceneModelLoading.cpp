#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "LightDirectional.h"
#include "LightPoint.h"

#include "Renderer.h"
#include "Camera.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"

#include "13_SceneModelLoading.h"

#define BACKPACK_PATH "resources/models/backpack/backpack.obj"
#define SPHERE_PATH "resources/models/sphere.fbx"

namespace scene {

    SceneModelLoading::SceneModelLoading()
        :Scene("Model Loading")
    {
        Renderer::Get().SetBackgroundColor(glm::vec4(0.18f, 0.23f, 0.24f, 1.0f));

        Camera& camera = Renderer::Get().GetActiveCamera();

        ////////////////////////////////////////////////////////////////////////////
        //                           directional light                            //
        ////////////////////////////////////////////////////////////////////////////
        directional_light = std::make_unique<DirectionalLight>(
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(-1.0f, -1.0f, -1.0f),
            glm::vec3(0.2f),
            glm::vec3(1.0f),
            glm::vec3(1.0f)
        );

        ////////////////////////////////////////////////////////////////////////////
        //                            geometry loading                            //
        ////////////////////////////////////////////////////////////////////////////
        object = std::make_unique<Model>(BACKPACK_PATH, "resources/shaders/02_LoadingMesh/object.vert", "resources/shaders/02_LoadingMesh/object.frag");
    }

    void SceneModelLoading::OnUpdate(double delta_time)
    {
        Camera& cam = Renderer::Get().GetActiveCamera();
        glm::vec3 cam_pos = cam.GetPosition();

        glm::mat4 projection, model, ModelView, MVP;
        projection = glm::perspective(glm::radians(cam.GetFov()), static_cast<float>(Renderer::Get().GetScreenWidth()) / static_cast<float>(Renderer::Get().GetScreenHeight()), 0.1f, 100.0f);

        directional_light->Update(projection, cam.GetViewMatrix());

        ////////////////////////////////////////////////////////////////////////////
        //                               geometry                                 //
        ////////////////////////////////////////////////////////////////////////////
        for (auto& m : object->GetMeshes())
        {
            model = glm::mat4(1.0);
            model = glm::scale(model, glm::vec3(0.2f));
            ModelView = cam.GetViewMatrix() * model;
            MVP = projection * ModelView;

            auto& material = m->GetMaterial();
            material.SetUniform("model", model);
            material.SetUniform("mvp", MVP);
            //material.SetUniformVec3("material.base_color", glm::vec3(1.0f, 0.0f, 0.0f));
            material.SetUniform("material.shininess", 32.0f);

            material.SetUniform("dir_light.direction", directional_light->GetDirection());
            material.SetUniform("dir_light.ambient", directional_light->GetAmbient());
            material.SetUniform("dir_light.diffuse", directional_light->GetDiffuse());
            material.SetUniform("dir_light.specular", directional_light->GetSpecular());
        }
    }

    void SceneModelLoading::OnRender()
    {
        Renderer::Get().SetBackgroundColor(glm::vec4(0.18f, 0.23f, 0.24f, 1.0f));

        ////////////////////////////////////////////////////////////////////////////
        //                            light rendering                             //
        ////////////////////////////////////////////////////////////////////////////
        directional_light->GetMaterial().Bind();
        Renderer::Get().Draw(directional_light->GetVertArray(), directional_light->GetIndexBuffer(), *directional_light->GetMaterial().GetShader());
        directional_light->GetMaterial().Unbind();


        ////////////////////////////////////////////////////////////////////////////
        //                          geometry rendering                            //
        ////////////////////////////////////////////////////////////////////////////
        Renderer::Get().Draw(*object);
    }

    void SceneModelLoading::OnImGuiRender() { }
}