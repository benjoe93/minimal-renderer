#include <glad/glad.h>

#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices)
{
    m_va = std::make_unique<VertexArray>();
    m_va->Bind();

    m_vb = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(Vertex));
    m_vb->Bind();

    m_ib = std::make_unique<IndexBuffer>(indices.data(), indices.size());
    m_ib->Bind();

    m_va->SetLayout(*m_vb, 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    m_va->SetLayout(*m_vb, 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
    m_va->SetLayout(*m_vb, 2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, TexCoords)));

    m_va->Unbind();
    m_vb->Unbind();
    m_ib->Unbind();
}

