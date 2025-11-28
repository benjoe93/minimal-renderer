#include <iostream>

#include "LightDirectional.h"

DirectionalLight::DirectionalLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : Light(position, ambient, diffuse, specular), m_direction(direction)
{
    std::cout << "Directional light {\n";
    std::cout << "  " << "Direction: { " << m_direction.x << ", " << m_direction.y << ", " << m_direction.z << " }\n";
    std::cout << "  " << "Ambient:   { " << m_ambient.x   << ", " << m_ambient.y   << ", " << m_ambient.z   << " }\n";
    std::cout << "  " << "Diffuse:   { " << m_diffuse.x   << ", " << m_diffuse.y   << ", " << m_diffuse.z   << " }\n";
    std::cout << "  " << "Specular:  { " << m_specular.x  << ", " << m_specular.y  << ", " << m_specular.z  << " }\n";
    std::cout << "}\n\n";
}
