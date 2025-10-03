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

#include "07_SceneMultipleLights.h"

static glm::vec3 cube_positions[] = {
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
static glm::vec3 point_light_pos[] = {
    glm::vec3(0.7f,  0.2f,  2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3(0.0f,  0.0f, -3.0f)
};
static glm::vec3 point_light_col[] = {
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 0.0f)
};

namespace scene {

    SceneMultipleLights::SceneMultipleLights(Renderer& in_renderer)
        :Scene(in_renderer),
        light_color         { 1.0f,  1.0f, 1.0f },
        object_color        { 1.0f,  0.5f, 0.31f },
        dir_light_direction { 0.0f, -1.0f, 0.0f },
        background_color    { 0.18f,  0.23f, 0.24f }
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

        Camera& camera = m_renderer.GetActiveCamera();

        // light setup
        directional_light = std::make_unique<DirectionalLight>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.2f), glm::vec3(1.0f), glm::vec3(1.0f));

        for (int ptl_id = 0; ptl_id < 4; ptl_id++)
        {
            glm::vec3 color = point_light_col[ptl_id];
            glm::vec3 position = point_light_pos[ptl_id];

            point_lights.push_back(std::make_unique<PointLight>(position, color * 0.2f , color, color));
            ptl_data.push_back(PointLightData({ position[0], position[1], position[2] }, { color[0], color[1], color[2] }));
        }

        spot_light = std::make_unique<SpotLight>(camera.GetPosition(), camera.GetDirection(), 25.0f, 30.0f, glm::vec3(0.2f), glm::vec3(1.0f), glm::vec3(1.0f));

        // object setup
        object_va = std::make_unique<VertexArray>();
        object_va->Bind();

        object_vb = std::make_unique<VertexBuffer>(vertices, buffer_size);
        object_vb->Bind();
        object_ib = std::make_unique<IndexBuffer>(indices, element_size);
        object_ib->Bind();

        object_va->SetLayout(*object_vb, 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        object_va->SetLayout(*object_vb, 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        object_va->SetLayout(*object_vb, 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

        for (int mat_id = 0; mat_id < 10; mat_id++)
        {
            object_materials.push_back(std::make_unique<Material>("resources/shaders/01_Lighting/04_MultipleLights/object.vert", "resources/shaders/01_Lighting/04_MultipleLights/object.frag"));

            // bind textures
            object_materials[mat_id]->AddTexture("resources/textures/container2.png", "material.diffuse", true);
            object_materials[mat_id]->AddTexture("resources/textures/container2_specular.png", "material.specular", true);
        }

        object_va->Unbind();
        object_vb->Unbind();
        object_ib->Unbind();
    }

    void SceneMultipleLights::OnUpdate(double delta_time)
    {
        Camera& cam = m_renderer.GetActiveCamera();
        glm::vec3 cam_pos = cam.GetPosition();

        glm::mat4 projection, model, ModelView, MVP;
        projection = glm::perspective(glm::radians(cam.GetFov()), static_cast<float>(m_renderer.state->scr_width) / static_cast<float>(m_renderer.state->scr_height), 0.1f, 100.0f);

        // directional light
        directional_light->SetDirection(glm::vec3(dir_light_direction[0], dir_light_direction[1], dir_light_direction[2]));
        directional_light->Update(projection, cam.GetViewMatrix());

        // point lights
        for (int ptl_id = 0; ptl_id < point_lights.size(); ptl_id++)
        {
            point_lights[ptl_id]->SetPosition(glm::vec3(ptl_data[ptl_id].position[0], ptl_data[ptl_id].position[1], ptl_data[ptl_id].position[2]));
            point_lights[ptl_id]->SetAmbient (glm::vec3(ptl_data[ptl_id].color[0] * 0.2f, ptl_data[ptl_id].color[1] * 0.2f, ptl_data[ptl_id].color[2] * 0.2f));
            point_lights[ptl_id]->SetDiffuse (glm::vec3(ptl_data[ptl_id].color[0], ptl_data[ptl_id].color[1], ptl_data[ptl_id].color[2]));
            point_lights[ptl_id]->SetSpecular(glm::vec3(ptl_data[ptl_id].color[0], ptl_data[ptl_id].color[1], ptl_data[ptl_id].color[2]));

            point_lights[ptl_id]->Update(projection, cam.GetViewMatrix());
        }

        // spot light
        spot_light->SetPosition(cam_pos);
        spot_light->SetDirection(cam.GetDirection());
        spot_light->Update(projection, cam.GetViewMatrix());

        // objects
        for (int obj_id = 0; obj_id < 10; obj_id++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cube_positions[obj_id]);
            float angle = 20.0f * obj_id;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            ModelView = cam.GetViewMatrix() * model;
            MVP = projection * ModelView;

            object_materials[obj_id]->SetUniformMat4("model", model);
            object_materials[obj_id]->SetUniformMat4("mvp", MVP);
            object_materials[obj_id]->SetUniformFloat("material.shininess", 32.f);
            object_materials[obj_id]->SetUniformVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
            object_materials[obj_id]->SetUniformVec3("u_viewPos", glm::vec3(cam_pos[0], cam_pos[1], cam_pos[2]));

            // light data
            object_materials[obj_id]->SetUniformVec3("dir_light.direction", directional_light->GetDirection());
            object_materials[obj_id]->SetUniformVec3("dir_light.ambient", directional_light->GetAmbient());
            object_materials[obj_id]->SetUniformVec3("dir_light.diffuse", directional_light->GetDiffuse());
            object_materials[obj_id]->SetUniformVec3("dir_light.specular", directional_light->GetSpecular());

            for (int ptl_id = 0; ptl_id < point_lights.size(); ptl_id++)
            {
                std::string current_light = "point_lights[" + std::to_string(ptl_id) + "].";

                object_materials[obj_id]->SetUniformVec3( current_light + "position",    point_lights[ptl_id]->GetPosition());
                object_materials[obj_id]->SetUniformFloat(current_light + "constant",    point_lights[ptl_id]->GetConstant());
                object_materials[obj_id]->SetUniformFloat(current_light + "linear",        point_lights[ptl_id]->GetLinear());
                object_materials[obj_id]->SetUniformFloat(current_light + "quadratic",    point_lights[ptl_id]->GetQuadratic());
                object_materials[obj_id]->SetUniformVec3( current_light + "ambient",    point_lights[ptl_id]->GetAmbient());
                object_materials[obj_id]->SetUniformVec3( current_light + "diffuse",    point_lights[ptl_id]->GetDiffuse());
                object_materials[obj_id]->SetUniformVec3( current_light + "specular",    point_lights[ptl_id]->GetSpecular());
            }

            object_materials[obj_id]->SetUniformVec3("spot_light.position",            spot_light->GetPosition());
            object_materials[obj_id]->SetUniformVec3("spot_light.direction",        spot_light->GetDirection());
            object_materials[obj_id]->SetUniformVec3("spot_light.ambient",            spot_light->GetAmbient());
            object_materials[obj_id]->SetUniformVec3("spot_light.diffuse",            spot_light->GetDiffuse());
            object_materials[obj_id]->SetUniformVec3("spot_light.specular",            spot_light->GetSpecular());
            object_materials[obj_id]->SetUniformFloat("spot_light.constant",        spot_light->GetConstant());
            object_materials[obj_id]->SetUniformFloat("spot_light.linear",            spot_light->GetLinear());
            object_materials[obj_id]->SetUniformFloat("spot_light.quadratic",        spot_light->GetQuadratic());
            object_materials[obj_id]->SetUniformFloat("spot_light.cut_off",            glm::cos(glm::radians(spot_light->GetInnerAngle()/2)));
            object_materials[obj_id]->SetUniformFloat("spot_light.outer_cut_off",    glm::cos(glm::radians(spot_light->GetOuterAngle()/2)));
        }
    }

    void SceneMultipleLights::OnRender()
    {
        m_renderer.SetBackgroundColor(glm::vec4(background_color[0], background_color[1], background_color[2], 1.0));

        // light rendering
        directional_light->GetMaterial()->Bind();
        m_renderer.Draw(*directional_light->GetVertArray(), *directional_light->GetIndexBuffer(), directional_light->GetMaterial()->GetShader());
        directional_light->GetMaterial()->Unbind();

        for (int ptl_id = 0; ptl_id < point_lights.size(); ptl_id++)
        {
            point_lights[ptl_id]->GetMaterial()->Bind();
            m_renderer.Draw(*point_lights[ptl_id]->GetVertArray(), *point_lights[ptl_id]->GetIndexBuffer(), point_lights[ptl_id]->GetMaterial()->GetShader());
            point_lights[ptl_id]->GetMaterial()->Unbind();
        }

        spot_light->GetMaterial()->Bind();
        m_renderer.Draw(*spot_light->GetVertArray(), *spot_light->GetIndexBuffer(), spot_light->GetMaterial()->GetShader());
        spot_light->GetMaterial()->Unbind();

        // object rendering
        for (int obj_id = 0; obj_id < 10; obj_id++)
        {
            object_materials[obj_id]->Bind();
            m_renderer.Draw(*object_va, *object_ib, object_materials[obj_id]->GetShader());
            object_materials[obj_id]->Unbind();
        }
    }

    void SceneMultipleLights::OnImGuiRender()
    {
        ImGui::Begin("Multiple lights");
        ImGui::ColorEdit3("Background", background_color);
        ImGui::Separator();
        ImGui::Text("Sun");
        ImGui::SliderFloat3("Dir", dir_light_direction, -1.0f, 1.0f, "%.2f");
        ImGui::Separator();

        for (int i = 0; i < ptl_data.size(); i++)
        {
            std::string name = "Point Light " + std::to_string(i);
            std::string pos_id = "##Position" + std::to_string(i);
            std::string col_id = "##Color" + std::to_string(i);
            ImGui::Text(name.c_str());
            ImGui::SliderFloat3(pos_id.c_str(), ptl_data[i].position, -12.0f, 12.0f, "%.2f");
            ImGui::ColorEdit3(col_id.c_str(), ptl_data[i].color);
            ImGui::Separator();
        }
        ImGui::End();
    }
}