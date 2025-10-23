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

#include "13_SceneModelLoading.h"

#define BACKPACK_PATH "resources/models/backpack/backpack.obj"
#define SPHERE_PATH "resources/models/sphere.fbx"

namespace scene {

    SceneModelLoading::SceneModelLoading(Renderer& in_renderer)
        :Scene(in_renderer, "Model Loading")
    {
        m_renderer.SetBackgroundColor(glm::vec4(0.18f, 0.23f, 0.24f, 1.0f));

        Camera& camera = m_renderer.GetActiveCamera();

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
        Camera& cam = m_renderer.GetActiveCamera();
        glm::vec3 cam_pos = cam.GetPosition();

        glm::mat4 projection, model, ModelView, MVP;
        projection = glm::perspective(glm::radians(cam.GetFov()), static_cast<float>(m_renderer.state->scr_width) / static_cast<float>(m_renderer.state->scr_height), 0.1f, 100.0f);

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
            material.SetUniformMat4("model", model);
            material.SetUniformMat4("mvp", MVP);
            //material.SetUniformVec3("material.base_color", glm::vec3(1.0f, 0.0f, 0.0f));
            material.SetUniformFloat("material.shininess", 32.0f);

            material.SetUniformVec3("dir_light.direction", directional_light->GetDirection());
            material.SetUniformVec3("dir_light.ambient", directional_light->GetAmbient());
            material.SetUniformVec3("dir_light.diffuse", directional_light->GetDiffuse());
            material.SetUniformVec3("dir_light.specular", directional_light->GetSpecular());
        }
    }

    void SceneModelLoading::OnRender()
    {
        m_renderer.SetBackgroundColor(glm::vec4(0.18f, 0.23f, 0.24f, 1.0f));

        ////////////////////////////////////////////////////////////////////////////
        //                            light rendering                             //
        ////////////////////////////////////////////////////////////////////////////
        directional_light->GetMaterial()->Bind();
        m_renderer.Draw(*directional_light->GetVertArray(), *directional_light->GetIndexBuffer(), directional_light->GetMaterial()->GetShader());
        directional_light->GetMaterial()->Unbind();


        ////////////////////////////////////////////////////////////////////////////
        //                          geometery rendering                           //
        ////////////////////////////////////////////////////////////////////////////
        m_renderer.Draw(*object);
    }

    void SceneModelLoading::OnImGuiRender()
    {
    }
}