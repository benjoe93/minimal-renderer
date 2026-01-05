#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "vendor/imgui/imgui.h"

#include "Camera.h"
#include "Texture.h"
#include "Material.h"
#include "Model.h"
#include "ResourceManager.h"

#include "21_SceneAdvancedGLSL.h"

namespace scene {
    SceneAdvancedGLSL::SceneAdvancedGLSL()
        :Scene("Advanced GLSL")
    {
        // create the buffer
        m_uniform_buffer = new UniformBufferObj(2 * sizeof(glm::mat4));
        // define the range of the buffer that links to a uniform binding point
        m_uniform_buffer->SetRange(0, 0, 2 * sizeof(glm::mat4));

        // store the projection matrix (we only do this once now) (note: we're not using zoom anymore by changing the FoV)
        Camera& cam = Renderer::Get().GetActiveCamera();
        glm::mat4 projection = glm::perspective(
            glm::radians(cam.GetFov()),
            static_cast<float>(Renderer::Get().GetScreenWidth()) / static_cast<float>(Renderer::Get().GetScreenHeight()),
            Renderer::Get().GetState().near_plane,
            Renderer::Get().GetState().far_plane);
        m_uniform_buffer->Bind();
        m_uniform_buffer->StoreData(0, sizeof(glm::mat4), glm::value_ptr(projection));
        m_uniform_buffer->Unbind();

        // Resources
        std::string plane = "resources/models/plane.fbx";
        std::string box = "resources/models/box.fbx";

        //std::string vert_path = "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.vert";
        std::string vert_path = "resources/shaders/03_AdvancedOpenGL/06_Advanced_GLSL/uniform_buffer_obj.vert";
        std::string frag_path = "resources/shaders/03_AdvancedOpenGL/06_Advanced_GLSL/uniform_buffer_obj.frag";
        
        Texture2D* metal_tex = ResourceManager::Get().GetTexture2D("resources/textures/metal.png");
        Texture2D* marble_tex = ResourceManager::Get().GetTexture2D("resources/textures/container.jpg");

        // Models
        Model* box0 = new Model(box, vert_path, frag_path, Transform(glm::vec3(-0.75f,  0.75f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f)));
        Model* box1 = new Model(box, vert_path, frag_path, Transform(glm::vec3( 0.75f,  0.75f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f)));
        Model* box2 = new Model(box, vert_path, frag_path, Transform(glm::vec3(-0.75f, -0.75f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f)));
        Model* box3 = new Model(box, vert_path, frag_path, Transform(glm::vec3( 0.75f, -0.75f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f)));
        m_objects.push_back(std::move(box0));
        m_objects.push_back(std::move(box1));
        m_objects.push_back(std::move(box2));
        m_objects.push_back(std::move(box3));

        // Material uniforms
        for (auto& mat : box0->GetMaterials()) {
            mat->SetUniform("color", glm::vec3(1.0f, 0.0f, 0.0f));
            mat->AddUniformBuffer("matrices", m_uniform_buffer);
        }

        for (auto& mat : box1->GetMaterials()) {
            mat->SetUniform("color", glm::vec3(0.0f, 1.0f, 0.0f));
            mat->AddUniformBuffer("matrices", m_uniform_buffer);
        }

        for (auto& mat : box2->GetMaterials()) {
            mat->SetUniform("color", glm::vec3(0.0f, 0.0f, 1.0f));
            mat->AddUniformBuffer("matrices", m_uniform_buffer);
        }

        for (auto& mat : box3->GetMaterials()) {
            mat->SetUniform("color", glm::vec3(1.0f, 1.0f, 0.0f));
            mat->AddUniformBuffer("matrices", m_uniform_buffer);
        }
    }

    SceneAdvancedGLSL::~SceneAdvancedGLSL()
    {
        for (Model* model : m_objects) {
            delete model;
        }
        m_objects.clear();
    }

    void SceneAdvancedGLSL::OnUpdate(double delta_time)
    {
        Renderer::Get().SetBackgroundColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
        Camera& cam = Renderer::Get().GetActiveCamera();

        glm::mat4 projection, ModelView, MVP, view;
        projection = glm::perspective(
            glm::radians(cam.GetFov()),
            static_cast<float>(Renderer::Get().GetScreenWidth()) / static_cast<float>(Renderer::Get().GetScreenHeight()),
            Renderer::Get().GetState().near_plane,
            Renderer::Get().GetState().far_plane);
        view = cam.GetViewMatrix();

        // set the view matrix in the uniform block - we only have to do this once per loop iteration.
        m_uniform_buffer->Bind();
        m_uniform_buffer->StoreData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        m_uniform_buffer->Unbind();

        // objects
        for (auto& obj : m_objects)
        {
            ModelView = cam.GetViewMatrix() * obj->GetModelMatrix();
            MVP = projection * ModelView;

            for (auto& mat : obj->GetMaterials()) {
                mat->SetUniform("model", obj->GetModelMatrix());
                //mat->SetUniform("mvp", MVP);
                //mat->SetUniform("projection", projection);
                //mat->SetUniform("view", view);
            }
        }
    }

    void SceneAdvancedGLSL::OnRender()
    {
        for (auto& obj : m_objects)
            Renderer::Get().Draw(*obj);
    }
}
