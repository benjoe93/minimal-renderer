#pragma once
#include <string>
#include <vector>

#include "Renderer.h"
#include "UniformBufferObject.h"

class Shader
{
private:
    GLuint m_renderer_id;
    std::string m_vertex_path;
    std::string m_fragment_path;
    std::string m_geometry_path;
    std::vector<std::pair<std::string, UniformBufferObj*>> u_uniform_buffers;
    mutable std::unordered_map<std::string, GLint> m_uniform_cache;

public:
    Shader(const std::string& vertex_path, const std::string& fragment_path);
    Shader(const std::string& vertex_path, const std::string& fragment_path, const std::string& geometry_path);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&&) noexcept = default;
    Shader& operator=(Shader&&) noexcept = default;

    void Bind() const;
    void Unbind() const;

    // utility uniform functions
    bool GetUniformLocation(const std::string& name, int& location) const;

    void SetUniform(const std::string& name, bool value);
    void SetUniform(const std::string& name, int value);
    void SetUniform(const std::string& name, float value);
    void SetUniform(const std::string& name, float x, float y, float z);
    void SetUniform(const std::string& name, float x, float y, float z, float w);
    void SetUniform(const std::string& name, const glm::mat4& value);

    void PrintShaderFiles() const;

    GLuint GetId() const { return m_renderer_id; }
    const std::string& GetVertexPath() const { return m_vertex_path; }
    const std::string& GetFragmentPath() const { return m_fragment_path; }

    void BindUniformBuffer(const std::string& block_name, UniformBufferObj* buffer);
    GLuint GetUniformBlockIndex(const std::string& block_name) const;
 };
