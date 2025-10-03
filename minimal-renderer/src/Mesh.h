#pragma once
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Material;
class Texture;

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh
{
private:
    std::unique_ptr<VertexArray>  m_va;
    std::unique_ptr<VertexBuffer> m_vb;
    std::unique_ptr<IndexBuffer>  m_ib;
    std::unique_ptr<Material>     m_material;

public:
    std::vector<Vertex>       m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture>      m_textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::unique_ptr<Material> material);

    VertexArray* GetVertexArray() { return m_va.get(); }
    VertexBuffer* GetVertexBuffer() { return m_vb.get(); }
    IndexBuffer* GetIndexBuffer() const { return m_ib.get(); }
    Material& GetMaterial() const { return *m_material; };
};

