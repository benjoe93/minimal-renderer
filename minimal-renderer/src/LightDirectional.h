#pragma once
#include "Renderer.h"
#include "Light.h"

class VertexBuffer;
class Material;

class DirectionalLight : public Light
{
private:
    glm::vec3 m_direction;

public:
    DirectionalLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);

    const glm::vec3& GetDirection() const { return m_direction; }
    void SetDirection(const glm::vec3& new_direction) { m_direction = new_direction; }
};
