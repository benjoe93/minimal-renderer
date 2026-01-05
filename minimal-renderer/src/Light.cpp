#include "Light.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Material.h"

constexpr float LIGHT_SCALE = 0.1f;

/* QUAD DEFINITION */
constexpr float vertices[] = {
    // Position            // UV
    // Front face (z = -0.5)
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,  // Bottom-left
     0.5f, -0.5f, -0.5f,   1.0f, 0.0f,  // Bottom-right
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f,  // Top-right
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,  // Top-left
};
constexpr GLuint indices[] = {
    0, 1, 2,    // Front face (z = -0.5)
    2, 3, 0,
};

constexpr size_t vertex_count = 4;
constexpr size_t index_count = 6;
constexpr size_t floats_per_vertex = 5;
constexpr size_t buffer_size = vertex_count * floats_per_vertex * sizeof(float);

Light::Light(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
    : m_position(position), m_ambient(ambient), m_diffuse(diffuse), m_specular(specular)
{
    m_va = std::make_unique<VertexArray>();
    m_va->Bind();

    m_vb = std::make_unique<VertexBuffer>(vertices, buffer_size);
    m_vb->Bind();

    m_ib = std::make_unique<IndexBuffer>(indices, index_count);
    m_ib->Bind();

    m_va->SetLayout(*m_vb, 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    m_va->SetLayout(*m_vb, 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));

    m_material = std::make_unique<Material>("resources/shaders/common/light.vert", "resources/shaders/common/light.frag");

    m_va->Unbind();
    m_vb->Unbind();
    m_ib->Unbind();
}

Light::~Light() = default;

void Light::Update(const glm::mat4& projection, const glm::mat4& camera_view)
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_position);
    model = glm::scale(model, glm::vec3(LIGHT_SCALE));

    const glm::mat4 MVP = projection * camera_view * model;

    m_material->SetUniform("mvp", MVP);
    m_material->SetUniform("u_lightColor", m_diffuse);
}
