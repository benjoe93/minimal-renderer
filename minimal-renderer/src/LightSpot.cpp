#include <iostream>

#include "Material.h"
#include "LightSpot.h"

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, float inner_angle, float outer_angle, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : Light(position, ambient, diffuse, specular),
    m_direction(direction),
    m_inner_angle(inner_angle),
    m_outer_angle(outer_angle)
{
    std::cout << "Spotlight {\n";
    std::cout << "  " << "Position:  { " << m_position.x << ", " << m_position.y << ", " << m_position.z << " }\n";
    std::cout << "  " << "Direction: { " << m_direction.x << ", " << m_direction.y << ", " << m_direction.z << " }\n";
    std::cout << "  " << "Ambient:   { " << m_ambient.x << ", " << m_ambient.y << ", " << m_ambient.z << " }\n";
    std::cout << "  " << "Diffuse:   { " << m_diffuse.x << ", " << m_diffuse.y << ", " << m_diffuse.z << " }\n";
    std::cout << "  " << "Specular:  { " << m_specular.x << ", " << m_specular.y << ", " << m_specular.z << " }\n";
    std::cout << "  " << "Inner Angle: " << m_inner_angle << "\n";
    std::cout << "  " << "Outer Angle: " << m_outer_angle << "\n";
    std::cout << "  " << "Const:       " << m_constant << "\n";
    std::cout << "  " << "Linear:      " << m_linear << "\n";
    std::cout << "  " << "Quadratic:   " << m_quadratic << "\n";
    std::cout << "}\n\n";
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

    m_material->SetUniform("mvp", MVP);
    m_material->SetUniform("u_lightColor", m_diffuse);
}
