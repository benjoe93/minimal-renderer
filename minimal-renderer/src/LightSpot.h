#pragma once
#include "Light.h"
#include "Renderer.h"

class VertexBuffer;
class Material;

class SpotLight : public Light
{
private:
    static constexpr float DEFAULT_CONSTANT = 1.0f;
    static constexpr float DEFAULT_LINEAR = 0.09f;
    static constexpr float DEFAULT_QUADRATIC = 0.032f;

    glm::vec3 m_direction;

    float m_inner_angle;
    float m_outer_angle;

    float m_constant  = DEFAULT_CONSTANT;
    float m_linear    = DEFAULT_LINEAR;
    float m_quadratic = DEFAULT_QUADRATIC;

public:
    SpotLight(const glm::vec3& position, const glm::vec3& direction, float inner_angle, float outer_angle, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);

    void Update(const glm::mat4& projection, const glm::mat4& camera_view) override;

    const glm::vec3& GetDirection() const { return m_direction; }
    float GetInnerAngle() const { return m_inner_angle; };
    float GetOuterAngle() const { return m_outer_angle; };
    float GetConstant() const { return m_constant; };
    float GetLinear() const { return m_linear; };
    float GetQuadratic() const { return m_quadratic; };


    void SetDirection(const glm::vec3& new_direction) { m_direction = new_direction; }
    void SetInnerAngle(const float new_angle) { m_inner_angle = new_angle; };
    void SetOuterAngle(const float new_angle) { m_outer_angle = new_angle; };
    void SetConstant(const float value) { m_constant = value; }
    void SetLinear(const float value) { m_linear = value; }
    void SetQuadratic(const float value) { m_quadratic = value; }
};