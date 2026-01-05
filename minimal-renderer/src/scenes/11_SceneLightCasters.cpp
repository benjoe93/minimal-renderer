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

#include "11_SceneLightCasters.h"

constexpr float vertices[] = {
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
constexpr unsigned int indices[] = {
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
constexpr size_t element_size = 36;
constexpr size_t buffer_size = element_size * 8 * sizeof(float);

constexpr glm::vec3 cube_positions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

namespace scene {
SceneLightCasters::SceneLightCasters()
    :Scene("Light Casters")
{
    ////////////////////////////////////////////////////////////////////////////
    //                            geometry setup                              //
    ////////////////////////////////////////////////////////////////////////////
    object_va = std::make_unique<VertexArray>();
    object_va->Bind();

    object_vb = std::make_unique<VertexBuffer>(vertices, buffer_size);
    object_vb->Bind();
    object_ib = std::make_unique<IndexBuffer>(indices, element_size);
    object_ib->Bind();

    object_va->SetLayout(*object_vb, 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    object_va->SetLayout(*object_vb, 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    object_va->SetLayout(*object_vb, 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));

    for (int i = 0; i < 10; i++)
    {
        object_materials.push_back(std::make_unique<Material>("resources/shaders/01_Lighting/03_LightCasters/object.vert", "resources/shaders/01_Lighting/03_LightCasters/object.frag"));

        object_materials[i]->AddTexture2D("resources/textures/container2.png", "material.diffuse", true);
        object_materials[i]->AddTexture2D("resources/textures/container2_specular.png", "material.specular", true);
    }

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

    light_va->SetLayout(*light_vb, 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);

    light_material = std::make_unique<Material>("resources/shaders/01_Lighting/03_LightCasters/light.vert", "resources/shaders/01_Lighting/03_LightCasters/light.frag");

    light_va->Unbind();
    light_vb->Unbind();
    light_ib->Unbind();
}

void SceneLightCasters::OnUpdate(double delta_time)
{

    Camera& cam = Renderer::Get().GetActiveCamera();
    glm::vec3 cam_pos = cam.GetPosition();

    glm::mat4 projection, model, ModelView, MVP;
    projection = glm::perspective(glm::radians(cam.GetFov()), static_cast<float>(Renderer::Get().GetScreenWidth()) / static_cast<float>(Renderer::Get().GetScreenHeight()), 0.1f, 100.0f);

    // geometry update
    ////////////////////////////////////////////////////////////////////////////
    //                           directional light                            //
    ////////////////////////////////////////////////////////////////////////////
    /*
    for (int i = 0; i < 10; i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, cube_positions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

        ModelView = cam->GetViewMatrix() * model;
        MVP = projection * ModelView;

        object_materials[i]->SetUniform("model", model);
        object_materials[i]->SetUniform("mvp", MVP);

        object_materials[i]->SetUniform("material.shininess", 32.f);

        object_materials[i]->SetUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        object_materials[i]->SetUniform("u_viewPos", glm::vec3(cam_pos[0], cam_pos[1], cam_pos[2]));
        object_materials[i]->SetUniform("light.direction", glm::vec3(-light_position[0], -light_position[1], -light_position[2])); // Directon is need to be flipped to get the direction from the fragment to the light
        object_materials[i]->SetUniform("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        object_materials[i]->SetUniform("light.diffuse", glm::vec3(light_color[0], light_color[1], light_color[2]));
        object_materials[i]->SetUniform("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        object_materials[i]->SetUniform("light.constant", 1.0f);
        object_materials[i]->SetUniform("light.linear", 0.09f);
        object_materials[i]->SetUniform("light.quadratic", 0.032f);

    }
    */

    ////////////////////////////////////////////////////////////////////////////
    //                              point light                               //
    ////////////////////////////////////////////////////////////////////////////
    /*
    for (int i = 0; i < 10; i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, cube_positions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

        ModelView = cam->GetViewMatrix() * model;
        MVP = projection * ModelView;

        object_materials[i]->SetUniform("model", model);
        object_materials[i]->SetUniform("mvp", MVP);

        object_materials[i]->SetUniform("material.shininess", 32.f);

        object_materials[i]->SetUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        object_materials[i]->SetUniform("u_viewPos", glm::vec3(cam_pos[0], cam_pos[1], cam_pos[2]));
        object_materials[i]->SetUniform("light.position", glm::vec3(light_position[0], light_position[1], light_position[2]));
        object_materials[i]->SetUniform("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        object_materials[i]->SetUniform("light.diffuse", glm::vec3(light_color[0], light_color[1], light_color[2]));
        object_materials[i]->SetUniform("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        object_materials[i]->SetUniform("light.constant", 1.0f);
        object_materials[i]->SetUniform("light.linear", 0.09f);
        object_materials[i]->SetUniform("light.quadratic", 0.032f);

    }
    */

    ////////////////////////////////////////////////////////////////////////////
    //                               spotlight                                //
    ////////////////////////////////////////////////////////////////////////////
    for (int i = 0; i < 10; i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, cube_positions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

        ModelView = cam.GetViewMatrix() * model;
        MVP = projection * ModelView;

        object_materials[i]->SetUniform("model", model);
        object_materials[i]->SetUniform("mvp", MVP);

        object_materials[i]->SetUniform("material.shininess", 32.f);

        object_materials[i]->SetUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        object_materials[i]->SetUniform("u_viewPos", glm::vec3(cam_pos[0], cam_pos[1], cam_pos[2]));

        object_materials[i]->SetUniform("light.position", cam_pos);
        object_materials[i]->SetUniform("light.direction", cam.GetDirection());
        object_materials[i]->SetUniform("light.cut_off", glm::cos(glm::radians(12.5f))); // sending cosine for perf reasons | cos(light_angle) == dot(light_dir, spot_dir)
        object_materials[i]->SetUniform("light.outer_cut_off", glm::cos(glm::radians(17.5f))); // sending cosine for perf reasons | cos(light_angle) == dot(light_dir, spot_dir)

        object_materials[i]->SetUniform("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        object_materials[i]->SetUniform("light.diffuse", glm::vec3(light_color[0], light_color[1], light_color[2]));
        object_materials[i]->SetUniform("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        object_materials[i]->SetUniform("light.constant", 1.0f);
        object_materials[i]->SetUniform("light.linear", 0.09f);
        object_materials[i]->SetUniform("light.quadratic", 0.032f);

    }

    ////////////////////////////////////////////////////////////////////////////
    //                              light update                              //
    ////////////////////////////////////////////////////////////////////////////
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(light_position[0], light_position[1], light_position[2]));
    model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

    ModelView = cam.GetViewMatrix() * model;
    MVP = projection * ModelView;
    
    light_material->SetUniform("mvp", MVP);

    light_material->SetUniform("u_lightColor", glm::vec3(light_color[0], light_color[1], light_color[2]));
}

void SceneLightCasters::OnRender()
{

    ////////////////////////////////////////////////////////////////////////////
    //                          geometry rendering                            //
    ////////////////////////////////////////////////////////////////////////////
    for (int i = 0; i < 10; i++)
    {
        object_materials[i]->Bind();
        Renderer::Get().Draw(*object_va, *object_ib, *object_materials[i]->GetShader());
        object_materials[i]->Unbind();
    }

    ////////////////////////////////////////////////////////////////////////////
    //                            light rendering                             //
    ////////////////////////////////////////////////////////////////////////////
    light_material->Bind();
    Renderer::Get().Draw(*light_va, *light_ib, *light_material->GetShader());
    light_material->Unbind();
}

void SceneLightCasters::OnImGuiRender()
{
    ImGui::Begin(m_name.c_str());
    ImGui::SliderFloat3("Light Position", light_position, -10.f, 10.f);

    ImGui::Separator();

    ImGui::ColorEdit3("Light Color", light_color);
    ImGui::ColorEdit3("Object Color", object_color);
    ImGui::End();
}
}