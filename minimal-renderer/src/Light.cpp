#include "Light.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Material.h"

/* QUAD DEFINITION */
static float vertices[] = {
    // Position            // UV
    // Front face (z = -0.5)
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,  // Bottom-left
     0.5f, -0.5f, -0.5f,   1.0f, 0.0f,  // Bottom-right
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f,  // Top-right
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,  // Top-left
};
static unsigned int indices[] = {
    0, 1, 2,    // Front face (z = -0.5)
    2, 3, 0,
};

static size_t element_size = 6;
static size_t buffer_size = element_size * 3 * sizeof(float);

Light::Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : m_position(position), m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_material(nullptr)
{
    m_va = std::make_unique<VertexArray>();
    m_va->Bind();

    m_vb = std::make_unique<VertexBuffer>(vertices, buffer_size);
    m_vb->Bind();

    m_ib = std::make_unique<IndexBuffer>(indices, element_size);
    m_ib->Bind();

    m_va->SetLayout(*m_vb, 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    m_va->SetLayout(*m_vb, 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    m_material = std::make_unique<Material>("resources/shaders/common/light.vert", "resources/shaders/common/light.frag");

    m_va->Unbind();
    m_vb->Unbind();
    m_ib->Unbind();
}

Light::~Light()
{
}

void Light::Update(glm::mat4 projection, glm::mat4 camera_view)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_position);
    model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

    glm::mat4 ModelView = camera_view * model;
    glm::mat4 MVP = projection * ModelView;

    m_material->SetUniformMat4("mvp", MVP);
    m_material->SetUniformVec3("u_lightColor", m_diffuse);
}
