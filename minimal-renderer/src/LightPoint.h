#pragma once
#include "Renderer.h"
#include "Light.h"

class VertexBuffer;
class Material;

class PointLight : public Light
{
private:
    static constexpr float DEFAULT_CONSTANT = 1.0f;
    static constexpr float DEFAULT_LINEAR = 0.09f;
    static constexpr float DEFAULT_QUADRATIC = 0.032f;

    float m_constant = DEFAULT_CONSTANT;
    float m_linear = DEFAULT_LINEAR;
    float m_quadratic = DEFAULT_QUADRATIC;

public:
    PointLight(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);

    float GetConstant() const { return m_constant; };
    float GetLinear() const { return m_linear; };
    float GetQuadratic() const { return m_quadratic; };

    void SetConstant(const float value) { m_constant = value; }
    void SetLinear(const float value) { m_linear = value; }
    void SetQuadratic(const float value) { m_quadratic = value; }
};