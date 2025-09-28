#include <iostream>

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Material.h"
#include "LightDirectional.h"

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

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
	: m_direction(direction),
	m_ambient(ambient),
	m_diffuse(diffuse),
	m_specular(specular)
{
	std::cout << "Directional light =======================\n";
	std::cout << "Direction: { " << m_direction.x << ", " << m_direction.y << ", " << m_direction.z << " }\n";
	std::cout << "Ambient:   { " << m_ambient.x   << ", " << m_ambient.y   << ", " << m_ambient.z   << " }\n";
	std::cout << "Diffuse:   { " << m_diffuse.x   << ", " << m_diffuse.y   << ", " << m_diffuse.z   << " }\n";
	std::cout << "Specular:  { " << m_specular.x  << ", " << m_specular.y  << ", " << m_specular.z  << " }\n\n";

	m_va = std::make_unique<VertexArray>();
	m_va->Bind();

	m_vb = std::make_unique<VertexBuffer>(vertices, buffer_size);
	m_vb->Bind();

	m_ib = std::make_unique<IndexBuffer>(indices, element_size);
	m_ib->Bind();

	m_va->SetLayout(*m_vb, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	m_material = std::make_unique<Material>("resources/shaders/01_Lighting/04_MultipleLights/light.vert", "resources/shaders/01_Lighting/04_MultipleLights/light.frag");

	m_va->Unbind();
	m_vb->Unbind();
	m_ib->Unbind();
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::Update(glm::mat4 projection, glm::mat4 camera_view)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, -m_direction);
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

	glm::mat4 ModelView = camera_view * model;
	glm::mat4 MVP = projection * ModelView;

	m_material->SetUniformMat4("mvp", MVP);
	m_material->SetUniformVec3("u_lightColor", m_diffuse);
}