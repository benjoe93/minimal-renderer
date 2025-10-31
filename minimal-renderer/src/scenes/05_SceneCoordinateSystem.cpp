#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "vendor/imgui/imgui.h"
#include "vendor/stb_image/stb_image.h"

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

#include "05_SceneCoordinateSystem.h"


static float vertices[] = {
    // positions          // uvs        // colors           
     0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,   // top right
     0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f,   1.0f, 1.0f, 0.0f    // top left 
};
static unsigned int indices[] = {  // note that we start from 0!
    3, 1, 0,  // first Triangle
    3, 2, 1   // second Triangle
};

namespace scene {
    SceneCoordinateSystem::SceneCoordinateSystem(Renderer& in_renderer)
        :Scene(in_renderer, "Coordinate system")
    {
        ////////////////////////////////////////////////////////////////////////////
        //                              load texture                              //
        ////////////////////////////////////////////////////////////////////////////

        tex1 = std::make_shared<Texture2D>("resources/textures/container.jpg", "texture1");
        tex2 = std::make_shared<Texture2D>("resources/textures/awesomeface.png", "texture2");
        tex1->Bind();
        tex2->Bind(1);

        ////////////////////////////////////////////////////////////////////////////
        //                              shader setup                              //
        ////////////////////////////////////////////////////////////////////////////
        
        shader = std::make_unique<Shader>("resources/shaders/00_GettingStarted/default.vert", "resources/shaders/00_GettingStarted/default.frag");
        shader->Bind();
        shader->SetUniform("texture1", 0);
        shader->SetUniform("texture2", 1);

        ////////////////////////////////////////////////////////////////////////////
        //                            geometery setup                             //
        ////////////////////////////////////////////////////////////////////////////

        // vertex array object
        vao = std::make_unique<VertexArray>();
        vao->Bind();

        // vertex buffer object
        vbo = std::make_unique <VertexBuffer>(vertices, 4 * 8 * sizeof(float));
        vbo->Bind();

        // element buffer object
        ebo = std::make_unique<IndexBuffer>(indices, 6);
        ebo->Bind();

        vao->SetLayout(*vbo, 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);                    // vertex position
        vao->SetLayout(*vbo, 1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));  // uv coords
        vao->SetLayout(*vbo, 2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));  // vertex color
        
        vbo->Unbind();
        vao->Unbind();
    }

    void SceneCoordinateSystem::OnUpdate(double delta_time)
    {}

    void SceneCoordinateSystem::OnRender()
    {
        m_renderer.SetBackgroundColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
        m_renderer.Clear(GL_COLOR_BUFFER_BIT);
        
        // Projection matrix
        float aspect = static_cast<float>(m_renderer.state.scr_width) / m_renderer.state.scr_height;
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, m_renderer.state.near_plane, m_renderer.state.far_plane);

        // View matrix
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        // Model matrix
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(location[0], location[1], location[2]));
        model = glm::rotate(model, glm::radians(rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));
        
        shader->SetUniform("model", model);
        shader->SetUniform("view", view);
        shader->SetUniform("projection", projection);
        m_renderer.Draw(*vao, *ebo, *shader);
    }

    void SceneCoordinateSystem::OnImGuiRender()
    {
        ImGui::Begin(m_name.c_str());
        ImGui::DragFloat3("Location", location, 0.01f, -1.0f, 1.0f, "%.2f");
        ImGui::DragFloat3("Rotation", rotation, 0.1f, -720.0f, 720.0f, "%.1f");
        ImGui::End();
    }
}