#pragma once
#include "Renderer.h"

class VertexBuffer;
class Material;

class SpotLight
{
private:
	glm::vec3 m_position;
	glm::vec3 m_direction;

	float m_inner_angle;
	float m_outer_angle;

	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;

	float m_constant	= 1.0f;
	float m_linear		= 0.09f;
	float m_quadratic	= 0.032f;

	std::unique_ptr<VertexArray>	m_va;
	std::unique_ptr<VertexBuffer>	m_vb;
	std::unique_ptr<IndexBuffer>	m_ib;
	std::unique_ptr<Material>		m_material;

public:
	SpotLight(glm::vec3 position, glm::vec3 direction, float inner_angle, float outer_angle, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	~SpotLight();

	void Update(glm::mat4 projection, glm::mat4 camera_view);

	inline glm::vec3 GetDirection() const { return m_direction; };
	inline glm::vec3 GetPosition() const { return m_position; };
	inline glm::vec3 GetAmbient() const { return m_ambient; };
	inline glm::vec3 GetDiffuse() const { return m_diffuse; };
	inline glm::vec3 GetSpecular() const { return m_specular; };
	inline float GetInnerAngle() const { return m_inner_angle; };
	inline float GetOuterAngle() const { return m_outer_angle; };
	inline float GetConstant() const { return m_constant; };
	inline float GetLinear() const { return m_linear; };
	inline float GetQuadratic() const { return m_quadratic; };

	inline VertexArray* GetVertArray() const { return m_va.get(); }
	inline IndexBuffer* GetIndexBuffer() const { return m_ib.get(); }
	inline Material* GetMaterial() const { return m_material.get(); }

	inline void SetDirection(glm::vec3 new_direction) { m_direction = new_direction; };
	inline void SetPosition(glm::vec3 new_position) { m_position = new_position; };
	inline void SetAmbient(glm::vec3 new_ambient) { m_ambient = new_ambient; };
	inline void SetDiffuse(glm::vec3 new_diffuse) { m_diffuse = new_diffuse; };
	inline void SetSpecular(glm::vec3 new_specular) { m_specular = new_specular; };

	inline void SetInnerAngle(float new_angle) { m_inner_angle = new_angle; };
	inline void SetOuterAngle(float new_angle) { m_outer_angle = new_angle; };
};