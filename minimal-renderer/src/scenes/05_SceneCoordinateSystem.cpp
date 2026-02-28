#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "vendor/imgui/imgui.h"

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"

#include "05_SceneCoordinateSystem.h"

#include "ResourceManager.h"
#include "SceneRegistry.h"


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

SceneCoordinateSystem::SceneCoordinateSystem()
    : Scene(StaticName())
{
    // material setup
    material = ResourceManager::Get().GetMaterial("coord_material");
    if (!material) {
        material = ResourceManager::Get().CreateMaterial("coord_material", "resources/shaders/00_GettingStarted/default.vert", "resources/shaders/00_GettingStarted/default.frag");
        material->AddTexture2D("resources/textures/container.jpg", "texture1", true);
        material->AddTexture2D("resources/textures/awesomeface.png", "texture2", true);
    }

    // geometry setup
    // vertex array object
    vao = std::make_unique<VertexArray>();
    vao->Bind();

    // vertex buffer object
    vbo = std::make_unique <VertexBuffer>(vertices, 4 * 8 * sizeof(float));
    vbo->Bind();

    // element buffer object
    ebo = std::make_unique<IndexBuffer>(indices, 6);
    ebo->Bind();

    vao->SetLayout(*vbo, 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);                                // vertex position
    vao->SetLayout(*vbo, 1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));  // uv coords
    vao->SetLayout(*vbo, 2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(5 * sizeof(float)));  // vertex color

    vbo->Unbind();
    vao->Unbind();
}

void SceneCoordinateSystem::OnUpdate(double delta_time)
{}

void SceneCoordinateSystem::OnRender()
{
    Renderer::Get().SetBackgroundColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
    Renderer::Get().Clear(GL_COLOR_BUFFER_BIT);

    // Projection matrix
    const auto& renderer = Renderer::Get();
    const float aspect = static_cast<float>(AppState::Get().GetScreenWidth()) / static_cast<float>(AppState::Get().GetScreenHeight());
    const glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, AppState::Get().GetNearPlane(), AppState::Get().GetFarPlane());

    // View matrix
    auto view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    // Model matrix
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(location[0], location[1], location[2]));
    model = glm::rotate(model, glm::radians(rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));

    material->SetUniform("model", model);
    material->SetUniform("view", view);
    material->SetUniform("projection", projection);
    Renderer::Get().Draw(*vao, *ebo, material);
}

void SceneCoordinateSystem::OnImGuiRender()
{
    ImGui::Begin(m_name.c_str());
    ImGui::DragFloat3("Location", location, 0.01f, -1.0f, 1.0f, "%.2f");
    ImGui::DragFloat3("Rotation", rotation, 0.1f, -720.0f, 720.0f, "%.1f");
    ImGui::End();
}

REGISTER_SCENE(SceneCoordinateSystem);