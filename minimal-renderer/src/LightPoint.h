#pragma once
#include "Renderer.h"
#include "Light.h"

class VertexBuffer;
class Material;

class PointLight : public Light
{
private:
    float m_constant = 1.0f;
    float m_linear = 0.09f;
    float m_quadratic = 0.032f;

public:
    PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    inline float GetConstant() const { return m_constant; };
    inline float GetLinear() const { return m_linear; };
    inline float GetQuadratic() const { return m_quadratic; };

    inline void SetPosition(glm::vec3 new_position) { m_position = new_position; };
};