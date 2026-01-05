#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "Material.h"
#include "LightSpot.h"

constexpr float LIGHT_SCALE = 0.1f;

SpotLight::SpotLight(const glm::vec3& position, const glm::vec3& direction, float inner_angle, float outer_angle, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
    : Light(position, ambient, diffuse, specular),
    m_direction(direction),
    m_inner_angle(inner_angle),
    m_outer_angle(outer_angle)
{
#ifndef NDEBUG
    std::cout << "Spotlight {\n";
    std::cout << "  Position:    " << glm::to_string(m_position)    << "\n";
    std::cout << "  Direction:   " << glm::to_string(m_direction)   << "\n";
    std::cout << "  Ambient:     " << glm::to_string(m_ambient)     << "\n";
    std::cout << "  Diffuse:     " << glm::to_string(m_diffuse)     << "\n";
    std::cout << "  Specular:    " << glm::to_string(m_specular)    << "\n";
    std::cout << "  Inner Angle: " << m_inner_angle << "\n";
    std::cout << "  Outer Angle: " << m_outer_angle << "\n";
    std::cout << "  Const:       " << m_constant    << "\n";
    std::cout << "  Linear:      " << m_linear      << "\n";
    std::cout << "  Quadratic:   " << m_quadratic   << "\n";
    std::cout << "}\n\n";
#endif
}

void SpotLight::Update(const glm::mat4& projection, const glm::mat4& camera_view)
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_position);

    constexpr auto up = glm::vec3(0.0f, 1.0f, 0.0f);
    const glm::vec3 target = m_position + m_direction;
    const glm::mat4 lookAtMatrix = glm::lookAt(m_position, target, up);

    // Extract rotation from lookAt
    const auto rotationMatrix = glm::mat3(lookAtMatrix);
    model = model * glm::mat4(rotationMatrix);
    model = glm::scale(model, glm::vec3(LIGHT_SCALE));

    const glm::mat4 MVP = projection * camera_view * model;

    m_material->SetUniform("mvp", MVP);
    m_material->SetUniform("u_lightColor", m_diffuse);
}
