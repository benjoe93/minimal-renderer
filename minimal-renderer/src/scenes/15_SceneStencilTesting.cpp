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

#include "15_SceneStencilTesting.h"



namespace scene {

    SceneStencilTesting::SceneStencilTesting(Renderer& in_renderer)
        :Scene(in_renderer, "Stencil Testing")
    {
        m_renderer.SetBackgroundColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
        
        m_renderer.SetDepthTest(true);
        m_renderer.SetDepthFunction(TestingFunc::LESS);                   // discard fragments that have higher values then the current (behind object)
        m_renderer.SetStencilTest(true);
        m_renderer.SetStencilFunction(TestingFunc::NOTEQUAL, 1, 0xFF);    // pass stencil test if stencil value != 1 (The mask 0xFF means all 8 bits are considered in the comparison)
        m_renderer.SetStencilOperation(StencilOp::KEEP, StencilOp::KEEP, StencilOp::REPLACE);

        Camera& camera = m_renderer.GetActiveCamera();

        // object setup
        objects.push_back(std::make_unique<Model>("resources/models/plane.fbx", "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.vert", "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.frag"));
        objects.push_back(std::make_unique<Model>("resources/models/box.fbx",   "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.vert", "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.frag"));
        objects.push_back(std::make_unique<Model>("resources/models/box.fbx",   "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.vert", "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.frag"));
        // boxes for the outline
        outline_objects.push_back(std::make_unique<Model>("resources/models/box.fbx",   "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/stencil_test.vert", "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/stencil_test.frag"));
        outline_objects.push_back(std::make_unique<Model>("resources/models/box.fbx",   "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/stencil_test.vert", "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/stencil_test.frag"));

        for (auto& mesh : objects[0]->GetMeshes())
            mesh->GetMaterial().AddTexture2D("resources/textures/metal.png", "material.diffuse", true);
        for (auto& mesh : objects[1]->GetMeshes())
            mesh->GetMaterial().AddTexture2D("resources/textures/marble.jpg", "material.diffuse", true);
        for (auto& mesh : objects[2]->GetMeshes())
            mesh->GetMaterial().AddTexture2D("resources/textures/marble.jpg", "material.diffuse", true);
    }

    void SceneStencilTesting::OnUpdate(double delta_time)
    {
        m_renderer.SetBackgroundColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

        Camera& cam = m_renderer.GetActiveCamera();
        glm::vec3 cam_pos = cam.GetPosition();

        glm::mat4 projection, model, ModelView, MVP;
        projection = glm::perspective(glm::radians(cam.GetFov()), static_cast<float>(m_renderer.state.scr_width) / static_cast<float>(m_renderer.state.scr_height), 0.1f, 100.0f);

        // objects
        // plane
        for (auto& m : objects[0]->GetMeshes())
        {
            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
            model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            ModelView = cam.GetViewMatrix() * model;
            MVP = projection * ModelView;

            auto& material = m->GetMaterial();
            material.SetUniform("model", model);
            material.SetUniform("mvp", MVP);
        }

        // boxes for rendering
        for (auto& m : objects[1]->GetMeshes())
        {
            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
            //model = glm::scale(model, glm::vec3(0.2f));
            ModelView = cam.GetViewMatrix() * model;
            MVP = projection * ModelView;

            auto& material = m->GetMaterial();
            material.SetUniform("model", model);
            material.SetUniform("mvp", MVP);
        }
        for (auto& m : objects[2]->GetMeshes())
        {
            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
            //model = glm::scale(model, glm::vec3(0.2f));
            ModelView = cam.GetViewMatrix() * model;
            MVP = projection * ModelView;

            auto& material = m->GetMaterial();
            material.SetUniform("model", model);
            material.SetUniform("mvp", MVP);
        }

        // boxes for outline
        for (auto& m : outline_objects[0]->GetMeshes())
        {
            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
            model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
            //model = glm::scale(model, glm::vec3(0.2f));
            ModelView = cam.GetViewMatrix() * model;
            MVP = projection * ModelView;

            auto& material = m->GetMaterial();
            material.SetUniform("model", model);
            material.SetUniform("mvp", MVP);
        }
        for (auto& m : outline_objects[1]->GetMeshes())
        {
            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
            //model = glm::scale(model, glm::vec3(0.2f));
            ModelView = cam.GetViewMatrix() * model;
            MVP = projection * ModelView;

            auto& material = m->GetMaterial();
            material.SetUniform("model", model);
            material.SetUniform("mvp", MVP);
        }
    }

    void SceneStencilTesting::OnRender()
    {
        m_renderer.SetDepthTest(true);
        m_renderer.SetStencilOperation(StencilOp::KEEP, StencilOp::KEEP, StencilOp::REPLACE); // replace stencil value when both stencil and depth tests pass

        // draw floor as normal, but don't write the floor to the stencil buffer, we only care about the containers. 
        // We set its mask to 0x00 to not write to the stencil buffer.
        m_renderer.SetStencilMask(0x00);                        // disable writing to stencil buffer (all bits masked)
        m_renderer.Draw(*objects[0]);                           // draw floor without affecting stencil

        // 1st. render pass, draw objects as normal, writing to the stencil buffer
        // --------------------------------------------------------------------
        m_renderer.SetStencilFunction(TestingFunc::ALWAYS, 1, 0xFF);      // always pass stencil test, set reference value to 1
        m_renderer.SetStencilMask(0xFF);                        // enable writing to stencil buffer (all bits unmasked)
        for (unsigned int i = 1; i < 3; i++)
        {
            m_renderer.Draw(*objects[i]);                       // draw objects and mark their pixels with 1 in stencil buffer
        }

        // 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
        // Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn, thus only drawing 
        // the objects' size differences, making it look like borders.
        // -----------------------------------------------------------------------------------------------------------------------------
        m_renderer.SetStencilFunction(TestingFunc::NOTEQUAL, 1, 0xFF);    // only pass stencil test where stencil value != 1 (around edges)
        m_renderer.SetStencilMask(0x00);                        // disable writing to stencil buffer (read-only mode)
        m_renderer.SetDepthTest(false);                         // disable depth test so outline draws on top of everything
        for (auto& obj : outline_objects)
        {
            m_renderer.Draw(*obj);                              // draw scaled objects, only visible where original wasn't drawn (outline effect)
        }
        m_renderer.SetStencilMask(0xFF);                        // re-enable stencil writing for future rendering
        m_renderer.SetStencilFunction(TestingFunc::ALWAYS, 0, 0xFF);      // reset stencil function to always pass with ref value 0
        m_renderer.SetDepthTest(true);                          // re-enable depth testing for normal rendering
    }

    void SceneStencilTesting::OnImGuiRender()
    {
    }
}