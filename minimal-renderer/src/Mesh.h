#pragma once
#include <iostream>
#include <vector>

#include <glm/glm.hpp>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture2D.h"

class Material;

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

    std::vector<Vertex>     m_vertices;
    std::vector<GLuint>     m_indices;

public:
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::unique_ptr<Material> material);
    ~Mesh() = default;

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&&) = default;
    Mesh& operator=(Mesh&&) = default;

    VertexArray& GetVertexArray() const { return *m_va; }
    VertexBuffer& GetVertexBuffer() const { return *m_vb; }
    IndexBuffer& GetIndexBuffer() const { return *m_ib; }
    Material& GetMaterial() const { return *m_material; }

    const std::vector<Vertex>& GetVertices() const { return m_vertices; }
    const std::vector<unsigned int>& GetIndices() const { return m_indices; }
};