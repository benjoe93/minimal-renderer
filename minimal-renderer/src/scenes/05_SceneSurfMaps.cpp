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
#include "Material.h"

#include "05_SceneSurfMaps.h"

namespace scene {
SceneSurfMaps::SceneSurfMaps(Renderer& in_renderer)
    :Scene(in_renderer),
    light_color        { 1.0f, 1.0f, 1.0f },
    object_color    { 1.0f, 0.5f, 0.31f },
    light_position    { 1.2f, 1.0f, 2.0f }
{
    /* QUAD DEFINITION */
    float vertices[] = {
        // Positions            // Normal                // uv
        // Front face (z = -0.5)
        -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
        // Back face (z = 0.5)
        -0.5f, -0.5f,  0.5f,     0.0f,  0.0f, 1.0f,        0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,     0.0f,  0.0f, 1.0f,        1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,     0.0f,  0.0f, 1.0f,        1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,     0.0f,  0.0f, 1.0f,        1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,     0.0f,  0.0f, 1.0f,        0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,     0.0f,  0.0f, 1.0f,        0.0f, 0.0f,
        // Left face (x = -0.5)
        -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
        // Right face (x = 0.5)
         0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
        // Bottom face (y = -0.5)
        -0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
        // Top face (y = 0.5)
        -0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,    0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,    1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,    0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,    0.0f, 1.0f
    };
    unsigned int indices[] = {
        // Front face
         0,  1,  2,
         3,  4,  5,
        // Back face
         6,  7,  8,
         9, 10, 11,
        // Left face
        12, 13, 14,
        15, 16, 17,
        // Right face
        18, 19, 20,
        21, 22, 23,
        // Bottom face
        24, 25, 26,
        27, 28, 29,
        // Top face
        30, 31, 32,
        33, 34, 35
    };

    size_t element_size = 36;
    size_t buffer_size = element_size * 8 * sizeof(float);

    // Object setup
    object_va = std::make_unique<VertexArray>();
    object_va->Bind();

    object_vb = std::make_unique<VertexBuffer>(vertices, buffer_size);
    object_vb->Bind();
    object_ib = std::make_unique<IndexBuffer>(indices, element_size);
    object_ib->Bind();

    object_va->SetLayout(*object_vb, 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    object_va->SetLayout(*object_vb, 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    object_va->SetLayout(*object_vb, 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    object_material = std::make_unique<Material>("resources/shaders/01_Lighting/02_SurfaceMaps/object.vert", "resources/shaders/01_Lighting/02_SurfaceMaps/object.frag");

    // Bind Textures
    object_material->AddTexture("resources/textures/container2.png", "material.diffuse", true);
    object_material->AddTexture("resources/textures/container2_specular.png", "material.specular", true);

    object_va->Unbind();
    object_vb->Unbind();
    object_ib->Unbind();

    // Light setup
    light_va = std::make_unique<VertexArray>();
    light_va->Bind();

    light_vb = std::make_unique<VertexBuffer>(vertices, buffer_size);
    light_vb->Bind();
    light_ib = std::make_unique<IndexBuffer>(indices, element_size);
    light_ib->Bind();

    light_va->SetLayout(*light_vb, 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

    light_material = std::make_unique<Material>("resources/shaders/01_Lighting/02_SurfaceMaps/light.vert", "resources/shaders/01_Lighting/02_SurfaceMaps/light.frag");

    light_va->Unbind();
    light_vb->Unbind();
    light_ib->Unbind();
}

void SceneSurfMaps::OnUpdate(double delta_time)
{
    Camera& cam = m_renderer.GetActiveCamera();
    glm::vec3 cam_pos = cam.GetPosition();
    glm::mat4 projection = glm::perspective(glm::radians(cam.GetFov()), static_cast<float>(m_renderer.state->scr_width) / static_cast<float>(m_renderer.state->scr_height), 0.1f, 100.0f);

    // Object
    glm::mat4 model = glm::mat4(1.0f);

    object_material->SetUniformMat4("model", model);
    object_material->SetUniformMat4("view", cam.GetViewMatrix());
    object_material->SetUniformMat4("projection", projection);

    object_material->SetUniformFloat("material.shininess", 32.f);

    object_material->SetUniformVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    object_material->SetUniformVec3("u_viewPos", glm::vec3(cam_pos[0], cam_pos[1], cam_pos[2]));
    object_material->SetUniformVec3("light.position", glm::vec3(light_position[0], light_position[1], light_position[2]));
    object_material->SetUniformVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    object_material->SetUniformVec3("light.diffuse", glm::vec3(light_color[0], light_color[1], light_color[2]));
    object_material->SetUniformVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    // Light
    glm::mat4 light_transform = glm::mat4(1.0f);
    light_transform = glm::translate(light_transform, glm::vec3(light_position[0], light_position[1], light_position[2]));
    light_transform = glm::scale(light_transform, glm::vec3(0.1f, 0.1f, 0.1f));
    
    light_material->SetUniformMat4("model", light_transform);
    light_material->SetUniformMat4("view", cam.GetViewMatrix());
    light_material->SetUniformMat4("projection", projection);

    light_material->SetUniformVec3("u_lightColor", glm::vec3(light_color[0], light_color[1], light_color[2]));
}

void SceneSurfMaps::OnRender()
{
    // Object rendering
    object_material->Bind();
    m_renderer.Draw(*object_va, *object_ib, object_material->GetShader());
    object_material->Unbind();

    // Light rendering
    light_material->Bind();
    m_renderer.Draw(*light_va, *light_ib, light_material->GetShader());
    light_material->Unbind();
}

void SceneSurfMaps::OnImGuiRender()
{
    ImGui::Begin("Colors");
    ImGui::SliderFloat3("Light Position", light_position, -10.f, 10.f);

    ImGui::Separator();

    ImGui::ColorEdit3("Light Color", light_color);
    ImGui::ColorEdit3("Object Color", object_color);
    ImGui::End();
}
}