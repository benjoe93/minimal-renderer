#include <glad/glad.h>

#include "Material.h"
#include "Texture.h"

#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::unique_ptr<Material> material)
    : m_vertices(std::move(vertices)), m_indices(std::move(indices)), m_material(std::move(material))
{
    m_va = std::make_unique<VertexArray>();
    m_va->Bind();

    m_vb = std::make_unique<VertexBuffer>(m_vertices.data(), m_vertices.size() * sizeof(Vertex));
    m_vb->Bind();

    m_ib = std::make_unique<IndexBuffer>(m_indices.data(), m_indices.size());
    m_ib->Bind();

    m_va->SetLayout(*m_vb, 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    m_va->SetLayout(*m_vb, 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    m_va->SetLayout(*m_vb, 2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    m_va->Unbind();
    m_vb->Unbind();
    m_ib->Unbind();
}

