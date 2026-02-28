#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "Camera.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

#include "07_SceneCubeMultiple.h"

#include "Material.h"
#include "ResourceManager.h"
#include "SceneRegistry.h"


constexpr float vertices[] = {
    // positions            // uvs        // colors
    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,   1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,   1.0f, 0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,

     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,    0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,    1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f,

    -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f
};
constexpr unsigned int indices[] = {
     2,  1,  0, // back
     5,  4,  3,
     6,  7,  8, // front
     9, 10, 11,
    12, 13, 14, // left
    15, 16, 17,
    20, 19, 18, // right
    23, 22, 21,
    24, 25, 26, // bottom
    27, 28, 29,
    32, 31, 30, // top
    35, 34, 33
};
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

SceneCubeMultiple::SceneCubeMultiple()
    : Scene(StaticName())
{
    // Set scene background color
    Renderer::Get().SetBackgroundColor({ 0.2f, 0.3f, 0.3f, 1.0f });

    // material setup
    material = ResourceManager::Get().GetMaterial("cube_material");
    if (!material) {
        material = ResourceManager::Get().CreateMaterial("cube_material", "resources/shaders/00_GettingStarted/default.vert", "resources/shaders/00_GettingStarted/default.frag");
        material->AddTexture2D("resources/textures/container.jpg", "texture_1");
        material->AddTexture2D("resources/textures/awesomeface.png", "texture_2");
    }

    // geometry setup
    va = std::make_unique<VertexArray>();
    va->Bind();

    unsigned int size = 36 * 8 * sizeof(float);
    vb = std::make_unique<VertexBuffer>(vertices, size);
    vb->Bind();
    ib = std::make_unique<IndexBuffer>(indices, 36);

    va->SetLayout(*vb, 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);                                // vertex position
    va->SetLayout(*vb, 1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));  // uv coords
    va->SetLayout(*vb, 2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(5 * sizeof(float)));  // vertex color

    vb->Unbind();
    va->Unbind();
}

void SceneCubeMultiple::OnRender()
{
    // Projection matrix
    const glm::mat4 projection = glm::perspective(
        glm::radians(AppState::Get().GetActiveCamera().GetFov()),
        static_cast<float>(AppState::Get().GetScreenWidth()) / static_cast<float>(AppState::Get().GetScreenHeight()),
        0.1f,
        100.0f);

    // Model matrix
    for (unsigned char i = 0; i < 10; i++)
    {
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, cube_positions[i]);
        // set rotation
        const float rotation = i * 20.0f;
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(1.0f, 0.3f, 0.5f));

        material->SetUniform("model", model);
        material->SetUniform("view", AppState::Get().GetActiveCamera().GetViewMatrix());
        material->SetUniform("projection", projection);

        // draw first triangle
        Renderer::Get().Draw(*va, *ib , material);
    }
}

REGISTER_SCENE(SceneCubeMultiple);