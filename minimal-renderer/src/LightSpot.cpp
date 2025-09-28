#include <iostream>

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Material.h"
#include "LightSpot.h"

/* QUAD DEFINITION */
static float vertices[] = {
	// Front face (z = -0.5)
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	// Back face (z = 0.5)
	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
};
static unsigned int indices[] = {
	0, 1, 2,	// Front face (z = -0.5)
	2, 3, 0,
	4, 5, 6,	// Back face (z = 0.5)
	6, 7, 4,
	7, 3, 0,	// Left face (x = -0.5)
	0, 4, 7,
	1, 5, 6,	// Right face (x = 0.5)
	6, 2, 1,
	4, 0, 1,	// Bottom face (y = -0.5)
	1, 5, 4,
	3, 7, 6,	// Top face (y = 0.5)
	6, 2, 3
};

static size_t element_size = 36;
static size_t buffer_size = element_size * 3 * sizeof(float);

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, float inner_angle, float outer_angle, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
	: m_position(position),
	m_direction(direction),
	m_inner_angle(inner_angle),
	m_outer_angle(outer_angle),
	m_ambient(ambient),
	m_diffuse(diffuse),
	m_specular(specular)
{
	m_va = std::make_unique<VertexArray>();
	m_va->Bind();

	m_vb = std::make_unique<VertexBuffer>(vertices, buffer_size);
	m_vb->Bind();
	m_ib = std::make_unique<IndexBuffer>(indices, element_size);
	m_ib->Bind();

	m_va->SetLayout(*m_vb, 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	m_material = std::make_unique<Material>("resources/shaders/01_Lighting/03_LightCasters/light.vert", "resources/shaders/01_Lighting/03_LightCasters/light.frag");

	m_va->Unbind();
	m_vb->Unbind();
	m_ib->Unbind();
}

SpotLight::~SpotLight()
{
}

void SpotLight::Update(glm::mat4 projection, glm::mat4 camera_view)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, m_position);

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // world up
	glm::vec3 target = m_position + m_direction;
	glm::mat4 lookAtMatrix = glm::lookAt(m_position, target, up);

	// Extract rotation from lookAt (remove translation)
	glm::mat3 rotationMatrix = glm::mat3(lookAtMatrix);
	model = model * glm::mat4(rotationMatrix);

	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

	glm::mat4 ModelView = camera_view * model;
	glm::mat4 MVP = projection * ModelView;

	m_material->SetUniformMat4("mvp", MVP);
	m_material->SetUniformVec3("u_lightColor", m_diffuse);
}
