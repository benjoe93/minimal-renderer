#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "vendor/imgui/imgui.h"

#include "Renderer.h"
#include "Camera.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Texture2D.h"
#include "Material.h"

#include "06_SceneCube.h"

#include "ResourceManager.h"
#include "SceneRegistry.h"


static float vertices[] = {
        // positions          // uvs        // colors
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,   // 0: front top right
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   // 1: front bottom right
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   // 2: front bottom left
        -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,   1.0f, 1.0f, 0.0f,   // 3: front top left

         0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   1.0f, 0.0f, 1.0f,   // 4: back top right
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f,   0.0f, 1.0f, 1.0f,   // 5: back bottom right
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   1.0f, 0.0f, 1.0f,   // 6: back bottom left
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   1.0f, 1.0f, 1.0f    // 7: back top left
    };
static unsigned int indices[] = {
    // Front face (z = +0.5)
    0, 2, 1,
    0, 3, 2,

    // Back face (z = -0.5)
    4, 5, 6,
    4, 6, 7,

    // Left face (x = -0.5)
    3, 7, 6,
    3, 6, 2,

    // Right face (x = +0.5)
    0, 1, 5,
    0, 5, 4,

    // Bottom face (y = -0.5)
    1, 2, 5,
    2, 6, 5,

    // Top face (y = +0.5)
    0, 4, 7,
    0, 7, 3
};

SceneCube::SceneCube()
    : Scene(StaticName())
{
    // Set scene background color
    Renderer::Get().SetBackgroundColor({ 0.2f, 0.3f, 0.3f, 1.0f });

    material = ResourceManager::Get().GetMaterial("cube_material");
    if (!material) {
        material = ResourceManager::Get().CreateMaterial("cube_material", "resources/shaders/00_GettingStarted/default.vert", "resources/shaders/00_GettingStarted/default.frag");
        material->AddTexture2D("resources/textures/container.jpg", "texture_1", true);
        material->AddTexture2D("resources/textures/awesomeface.png", "texture_2", true);
    }

    // geometry setup
    va = std::make_unique<VertexArray>();
    va->Bind();

    unsigned int size = 8 * 8 * sizeof(float);
    vb = std::make_unique<VertexBuffer>(vertices, size);
    vb->Bind();
    ib = std::make_unique<IndexBuffer>(indices, 36);

    va->SetLayout(*vb, 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);                                // vertex position
    va->SetLayout(*vb, 1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));  // uv coords
    va->SetLayout(*vb, 2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(5 * sizeof(float)));  // vertex color

    vb->Unbind();
    va->Unbind();
}

void SceneCube::OnRender()
{
    // Projection matrix
    const glm::mat4 projection = glm::perspective(
        glm::radians(AppState::Get().GetActiveCamera().GetFov()),
        static_cast<float>(AppState::Get().GetScreenWidth()) / static_cast<float>(AppState::Get().GetScreenHeight()),
        0.1f,
        100.0f
    );

    // Model matrix
    auto model = glm::mat4(1.0f);
    // transform with user input
    model = glm::translate(model, glm::vec3(location[0], location[1], location[2]));
    model = glm::rotate(model, glm::radians(rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));

    material->SetUniform("model", model);
    material->SetUniform("view", AppState::Get().GetActiveCamera().GetViewMatrix());
    material->SetUniform("projection", projection);

    // draw first triangle
    Renderer::Get().Draw(*va, *ib , material);
}

void SceneCube::OnImGuiRender()
{
    ImGui::Begin(m_name.c_str());

    ImGui::Text("Translate");
    ImGui::SameLine();
    ImGui::DragFloat3("##offset", location, 0.01f);

    ImGui::Text("Rotation ");
    ImGui::SameLine();
    ImGui::DragFloat3("##rotation", rotation, 0.1f);
    
    ImGui::End();
}

REGISTER_SCENE(SceneCube);