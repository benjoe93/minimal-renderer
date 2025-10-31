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

#include "10_SceneSurfMaps.h"

static float vertices[] = {
    // Positions            // Normal               // uv
    // Front face (z = -0.5)
    -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
    // Back face (z = 0.5)
    -0.5f, -0.5f,  0.5f,     0.0f,  0.0f, 1.0f,     0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,     0.0f,  0.0f, 1.0f,     1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,     0.0f,  0.0f, 1.0f,     1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,     0.0f,  0.0f, 1.0f,     1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,     0.0f,  0.0f, 1.0f,     0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,     0.0f,  0.0f, 1.0f,     0.0f, 0.0f,
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
     -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
      0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
      0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
     -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
     -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
     // Top face (y = 0.5)
     -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f,
      0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
     -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 0.0f,
     -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f
};
static unsigned int indices[] = {
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
static size_t element_size = 36;
static size_t buffer_size = element_size * 8 * sizeof(float);

namespace scene {
SceneSurfMaps::SceneSurfMaps(Renderer& in_renderer)
    :Scene(in_renderer, "Surface Maps")
{
    ////////////////////////////////////////////////////////////////////////////
    //                            geometery setup                             //
    ////////////////////////////////////////////////////////////////////////////
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

    object_material->AddTexture2D("resources/textures/container2.png", "material.diffuse", true);
    object_material->AddTexture2D("resources/textures/container2_specular.png", "material.specular", true);

    object_va->Unbind();
    object_vb->Unbind();
    object_ib->Unbind();

    ////////////////////////////////////////////////////////////////////////////
    //                              light setup                               //
    ////////////////////////////////////////////////////////////////////////////
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
    glm::mat4 projection = glm::perspective(glm::radians(cam.GetFov()), static_cast<float>(m_renderer.state.scr_width) / static_cast<float>(m_renderer.state.scr_height), 0.1f, 100.0f);

    ////////////////////////////////////////////////////////////////////////////
    //                           geometery update                             //
    ////////////////////////////////////////////////////////////////////////////
    glm::mat4 model = glm::mat4(1.0f);

    object_material->SetUniform("model", model);
    object_material->SetUniform("view", cam.GetViewMatrix());
    object_material->SetUniform("projection", projection);

    object_material->SetUniform("material.shininess", 32.f);

    object_material->SetUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    object_material->SetUniform("u_viewPos", glm::vec3(cam_pos[0], cam_pos[1], cam_pos[2]));
    object_material->SetUniform("light.position", glm::vec3(light_position[0], light_position[1], light_position[2]));
    object_material->SetUniform("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    object_material->SetUniform("light.diffuse", glm::vec3(light_color[0], light_color[1], light_color[2]));
    object_material->SetUniform("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    ////////////////////////////////////////////////////////////////////////////
    //                             light update                               //
    ////////////////////////////////////////////////////////////////////////////
    glm::mat4 light_transform = glm::mat4(1.0f);
    light_transform = glm::translate(light_transform, glm::vec3(light_position[0], light_position[1], light_position[2]));
    light_transform = glm::scale(light_transform, glm::vec3(0.1f, 0.1f, 0.1f));
    
    light_material->SetUniform("model", light_transform);
    light_material->SetUniform("view", cam.GetViewMatrix());
    light_material->SetUniform("projection", projection);

    light_material->SetUniform("u_lightColor", glm::vec3(light_color[0], light_color[1], light_color[2]));
}

void SceneSurfMaps::OnRender()
{
    ////////////////////////////////////////////////////////////////////////////
    //                          geometery rendering                           //
    ////////////////////////////////////////////////////////////////////////////
    object_material->Bind();
    m_renderer.Draw(*object_va, *object_ib, object_material->GetShader());
    object_material->Unbind();

    ////////////////////////////////////////////////////////////////////////////
    //                            light rendering                             //
    ////////////////////////////////////////////////////////////////////////////
    light_material->Bind();
    m_renderer.Draw(*light_va, *light_ib, light_material->GetShader());
    light_material->Unbind();
}

void SceneSurfMaps::OnImGuiRender()
{
    ImGui::Begin(m_name.c_str());
    ImGui::DragFloat3("Light Position", light_position, 0.1f, -10.f, 10.f, "%.1f");

    ImGui::Separator();

    ImGui::ColorEdit3("Light Color", light_color);
    ImGui::ColorEdit3("Object Color", object_color);
    ImGui::End();
}
}