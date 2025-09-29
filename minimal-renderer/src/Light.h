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

    inline glm::vec3 GetPosition() const { return m_position; };
    inline glm::vec3 GetAmbient() const { return m_ambient; };
    inline glm::vec3 GetDiffuse() const { return m_diffuse; };
    inline glm::vec3 GetSpecular() const { return m_specular; };

    inline VertexArray* GetVertArray() const { return m_va.get(); }
    inline IndexBuffer* GetIndexBuffer() const { return m_ib.get(); }
    inline Material* GetMaterial() const { return m_material.get(); }

    inline void SetPosition(glm::vec3 new_position) { m_position = new_position; };
    inline void SetAmbient(glm::vec3 new_ambient) { m_ambient = new_ambient; };
    inline void SetDiffuse(glm::vec3 new_diffuse) { m_diffuse = new_diffuse; };
    inline void SetSpecular(glm::vec3 new_specular) { m_specular = new_specular; };
};