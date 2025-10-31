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
    Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    virtual ~Light();

    virtual void Update(glm::mat4 projection, glm::mat4 camera_view);

    glm::vec3 GetPosition() const { return m_position; };
    glm::vec3 GetAmbient() const { return m_ambient; };
    glm::vec3 GetDiffuse() const { return m_diffuse; };
    glm::vec3 GetSpecular() const { return m_specular; };

    VertexArray& GetVertArray() const { return *m_va.get(); }
    IndexBuffer& GetIndexBuffer() const { return *m_ib.get(); }
    Material& GetMaterial() const { return *m_material.get(); }

    void SetPosition(glm::vec3 new_position) { m_position = new_position; };
    void SetAmbient(glm::vec3 new_ambient) { m_ambient = new_ambient; };
    void SetDiffuse(glm::vec3 new_diffuse) { m_diffuse = new_diffuse; };
    void SetSpecular(glm::vec3 new_specular) { m_specular = new_specular; };
};