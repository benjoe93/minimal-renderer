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

#include "04_SceneTransformation.h"


static float vertices[] = {
    // positions          // uvs        // colors           
     0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,   // top right
     0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f,   1.0f, 1.0f, 0.0f    // top left 
};
static unsigned int indices[] = {
    3, 1, 0,  // first Triangle
    3, 2, 1   // second Triangle
};

namespace scene {
    SceneTransfrom::SceneTransfrom()
        :Scene("Model transformation")
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
        shader = std::make_unique<Shader>("resources/shaders/00_GettingStarted/transform.vert", "resources/shaders/00_GettingStarted/texture_loading.frag");
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

    void SceneTransfrom::OnUpdate(double delta_time)
    {}

    void SceneTransfrom::OnRender()
    {
        // clean new frame
        Renderer::Get().SetBackgroundColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
        Renderer::Get().Clear(GL_COLOR_BUFFER_BIT);
        
        // tranform
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(location[0], location[1], 0.0f));
        trans = glm::rotate(trans, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        
        // draw
        shader->SetUniform("transform", trans);
        Renderer::Get().Draw(*vao, *ebo, *shader);
    }

    void SceneTransfrom::OnImGuiRender()
    {
        ImGui::Begin(m_name.c_str());
        ImGui::DragFloat2("Location", location, 0.01f, -1.0f, 1.0f, "%.2f");
        ImGui::DragFloat("Rotation", &angle, 0.1f, -720.0f, 720.0f, "%.1f");
        ImGui::End();
    }
}