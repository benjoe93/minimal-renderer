#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "LightPoint.h"

PointLight::PointLight(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
    : Light(position, ambient, diffuse, specular)
{
#ifndef NDEBUG
    std::cout << "Point light {\n";
    std::cout << "  Position:  " << glm::to_string(m_position)  << "\n";
    std::cout << "  Ambient:   " << glm::to_string(m_ambient)   << "\n";
    std::cout << "  Diffuse:   " << glm::to_string(m_diffuse)   << "\n";
    std::cout << "  Specular:  " << glm::to_string(m_specular)  << "\n";
    std::cout << "  Const:     " << m_constant  << "\n";
    std::cout << "  Linear:    " << m_linear    << "\n";
    std::cout << "  Quadratic: " << m_quadratic << "\n";
    std::cout << "}\n\n";
#endif
}
