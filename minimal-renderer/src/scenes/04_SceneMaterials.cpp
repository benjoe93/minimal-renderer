#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "vendor/imgui/imgui.h"

#include "Renderer.h"
#include "Camera.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "Shader.h"

#include "04_SceneMaterials.h"

namespace scene {

SceneMaterials::SceneMaterials(Renderer& in_renderer)
    :Scene(in_renderer, "Materials"),
    light_color        { 1.0f, 1.0f, 1.0f },
    object_color    { 1.0f, 0.5f, 0.31f },
    light_position    { 1.2f, 1.0f, 2.0f }
{
    /* QUAD DEFINITION */
    float vertices[] = {
        // Positions            Normal
        // Front face (z = -0.5)
        -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
        // Back face (z = 0.5)
        -0.5f, -0.5f,  0.5f,     0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,     0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,     0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,     0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,     0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,     0.0f,  0.0f, 1.0f,
        // Left face (x = -0.5)
        -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,
        // Right face (x = 0.5)
         0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,
         // Bottom face (y = -0.5)
         -0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,
          0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,
          0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,
          0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,
         -0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,
         -0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,
         // Top face (y = 0.5)
         -0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,
          0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,
          0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,
          0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,
         -0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,
         -0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f
    };
    unsigned int indices[] = {
        // Front face
         2,  1,  0,
         5,  4,  3,
         // Back face
         6,  7,  8,
         9, 10, 11,
        // Left face
        12, 13, 14,
        15, 16, 17,
        // Right face
        20, 19, 18,
        23, 22, 21,
        // Bottom face
        24, 25, 26,
        27, 28, 29,
        // Top face
        32, 31, 30,
        35, 34, 33
    };

    size_t element_size = 36;
    size_t buffer_size = element_size * 6 * sizeof(float);

    // Object setup
    object_va = std::make_unique<VertexArray>();
    object_va->Bind();

    object_vb = std::make_unique<VertexBuffer>(vertices, buffer_size);
    object_vb->Bind();
    object_ib = std::make_unique<IndexBuffer>(indices, element_size);
    object_ib->Bind();

    object_va->SetLayout(*object_vb, 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    object_va->SetLayout(*object_vb, 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    object_shader = std::make_unique<Shader>("resources/shaders/01_Lighting/01_Materials/object.vert", "resources/shaders/01_Lighting/01_Materials/object.frag");

    object_va->Unbind();
    object_vb->Unbind();
    object_ib->Unbind();
    object_shader->Unbind();

    // Light setup
    light_va = std::make_unique<VertexArray>();
    light_va->Bind();

    light_vb = std::make_unique<VertexBuffer>(vertices, buffer_size);
    light_vb->Bind();
    light_ib = std::make_unique<IndexBuffer>(indices, element_size);
    light_ib->Bind();

    light_va->SetLayout(*light_vb, 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    //light_va->SetLayout(*light_vb, 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(6 * sizeof(float)));

    light_shader = std::make_unique<Shader>("resources/shaders/01_Lighting/01_Materials/light.vert", "resources/shaders/01_Lighting/01_Materials/light.frag");
    light_shader->Bind();

    light_va->Unbind();
    light_vb->Unbind();
    light_ib->Unbind();
    light_shader->Unbind();
}

void SceneMaterials::OnRender()
{
    Camera& cam = m_renderer.GetActiveCamera();
    glm::vec3 cam_pos = cam.GetPosition();

    // Object rendering
    glm::mat4 projection = glm::perspective(glm::radians(cam.GetFov()), static_cast<float>(m_renderer.state->scr_width) / static_cast<float>(m_renderer.state->scr_height), 0.1f, 100.0f);
    glm::mat4 model = glm::mat4(1.0f);
    //model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
    glm::mat4 mvp_matrix = projection * cam.GetViewMatrix() * model;

    object_shader->Bind();
    object_shader->SetMat4("model", model);
    object_shader->SetMat4("view", cam.GetViewMatrix());
    object_shader->SetMat4("projection", projection);

    object_shader->SetVec3("material.ambient", 1.0f, 0.5f, 0.31f);
    object_shader->SetVec3("material.diffuse", object_color[0], object_color[1], object_color[2]);
    object_shader->SetVec3("material.specular", 0.5f, 0.5f, 0.5f);
    object_shader->SetFloat("material.shininess", 32.f);

    object_shader->SetVec3("u_viewPos", cam_pos[0], cam_pos[1], cam_pos[2]);

    object_shader->SetVec3("light.position", light_position[0], light_position[1], light_position[2]);
    object_shader->SetVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    object_shader->SetVec3("light.diffuse", light_color[0], light_color[1], light_color[2]);
    object_shader->SetVec3("light.specular", 1.0f, 1.0f, 1.0f);

    m_renderer.Draw(*object_va, *object_ib, *object_shader);

    // Light rendering
    light_shader->Bind();
    glm::mat4 light_transform = glm::mat4(1.0f);
    light_transform = glm::translate(light_transform, glm::vec3(light_position[0], light_position[1], light_position[2]));
    light_shader->SetMat4("model", light_transform);
    light_shader->SetMat4("view", cam.GetViewMatrix());
    light_shader->SetMat4("projection", projection);

    light_shader->SetVec3("u_lightColor", light_color[0], light_color[1], light_color[2]);

    m_renderer.Draw(*light_va, *light_ib, *light_shader);
}

void SceneMaterials::OnImGuiRender()
{
    ImGui::Begin("Colors");
    ImGui::SliderFloat3("Light Position", light_position, -10.f, 10.f);

    ImGui::Separator();

    ImGui::ColorEdit3("Light Color", light_color);
    ImGui::ColorEdit3("Object Color", object_color);
    ImGui::End();
}
}