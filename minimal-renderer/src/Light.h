#pragma once
#include <iostream>
#include "Renderer.h"

class VertexArray;
class VertexBuffer;
class IndexBuffer;
class Material;

class Light
{
protected:
    glm::vec3 m_position;

    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;

    std::unique_ptr<VertexArray>    m_va;
    std::unique_ptr<VertexBuffer>   m_vb;
    std::unique_ptr<IndexBuffer>    m_ib;
    std::unique_ptr<Material>       m_material;

public:
    Light(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);
    virtual ~Light();

    virtual void Update(const glm::mat4& projection, const glm::mat4& camera_view);

    const glm::vec3& GetPosition() const { return m_position; };
    const glm::vec3& GetAmbient() const { return m_ambient; };
    const glm::vec3& GetDiffuse() const { return m_diffuse; };
    const glm::vec3& GetSpecular() const { return m_specular; };

    VertexArray& GetVertArray() const { return *m_va; }
    IndexBuffer& GetIndexBuffer() const { return *m_ib; }
    Material* GetMaterial() const { return m_material.get(); }

    void SetPosition(const glm::vec3& new_position) { m_position = new_position; };
    void SetAmbient(const glm::vec3& new_ambient) { m_ambient = new_ambient; };
    void SetDiffuse(const glm::vec3& new_diffuse) { m_diffuse = new_diffuse; };
    void SetSpecular(const glm::vec3& new_specular) { m_specular = new_specular; };
};