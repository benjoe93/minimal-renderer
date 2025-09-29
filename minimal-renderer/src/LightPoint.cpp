#include <iostream>

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Material.h"
#include "LightPoint.h"

PointLight::PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : Light(position, ambient, diffuse, specular)
{
    std::cout << "Point light {\n";
    std::cout << "  " << "Position:  { " << m_position.x << ", " << m_position.y << ", " << m_position.z << " }\n";
    std::cout << "  " << "Ambient:   { " << m_ambient.x  << ", " << m_ambient.y  << ", " << m_ambient.z  << " }\n";
    std::cout << "  " << "Diffuse:   { " << m_diffuse.x  << ", " << m_diffuse.y  << ", " << m_diffuse.z  << " }\n";
    std::cout << "  " << "Specular:  { " << m_specular.x << ", " << m_specular.y << ", " << m_specular.z << " }\n";
    std::cout << "  " << "Const:       " << m_constant     << "\n";
    std::cout << "  " << "Linear:      " << m_linear     << "\n";
    std::cout << "  " << "Quadratic:   " << m_quadratic  << "\n\n";
    std::cout << "}\n\n";
}