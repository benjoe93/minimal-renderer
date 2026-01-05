#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "LightDirectional.h"

DirectionalLight::DirectionalLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
    : Light(position, ambient, diffuse, specular), m_direction(direction)
{
#ifndef NDEBUG
    std::cout << "Directional light {\n";
    std::cout << "  Direction: " << glm::to_string(m_direction) << "\n";
    std::cout << "  Ambient:   " << glm::to_string(m_ambient) << "\n";
    std::cout << "  Diffuse:   " << glm::to_string(m_diffuse) << "\n";
    std::cout << "  Specular:  " << glm::to_string(m_specular) << "\n";
    std::cout << "}\n\n";
#endif
}
