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
    DirectionalLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    inline glm::vec3 GetDirection() const { return m_direction; };
    inline void SetDirection(glm::vec3 new_direction) { m_direction = new_direction; };
};
