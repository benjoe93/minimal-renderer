#pragma once
#include "Light.h"
#include "Renderer.h"

class VertexBuffer;
class Material;

class SpotLight : public Light
{
private:
    glm::vec3 m_direction;

    float m_inner_angle;
    float m_outer_angle;

    float m_constant  = 1.0f;
    float m_linear    = 0.09f;
    float m_quadratic = 0.032f;

public:
    SpotLight(glm::vec3 position, glm::vec3 direction, float inner_angle, float outer_angle, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    void Update(glm::mat4 projection, glm::mat4 camera_view) override;

    glm::vec3 GetDirection() const { return m_direction; };
    float GetInnerAngle() const { return m_inner_angle; };
    float GetOuterAngle() const { return m_outer_angle; };
    float GetConstant() const { return m_constant; };
    float GetLinear() const { return m_linear; };
    float GetQuadratic() const { return m_quadratic; };


    void SetDirection(glm::vec3 new_direction) { m_direction = new_direction; };
    void SetInnerAngle(float new_angle) { m_inner_angle = new_angle; };
    void SetOuterAngle(float new_angle) { m_outer_angle = new_angle; };
};