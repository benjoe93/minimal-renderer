#pragma once
#include <string>

#include "Renderer.h"
#include "Texture.h"
#include "UniformBufferObject.h"


class Shader
{
private:
    unsigned int m_renderer_id;
    std::string& m_vertex_path;
    std::string& m_fragment_path;
    std::vector<std::pair<std::string, UniformBufferObj*>> u_uniform_buffers;

public:
    Shader(std::string vertex_path, std::string fragment_path);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    // utility uniform functions
    bool GetUniformLocation(const std::string& name, int& location) const;

    void SetUniform(const std::string& name, bool value);
    void SetUniform(const std::string& name, int value);
    void SetUniform(const std::string& name, float value);
    void SetUniform(const std::string& name, const float x, const float y, const float z);
    void SetUniform(const std::string& name, const float x, const float y, const float z, const float w);
    void SetUniform(const std::string& name, glm::mat4 value);

    void PrintShaderFiles() const;

    const std::string& GetVertexPath() const { return m_vertex_path; }
    const std::string& GetFragmentPath() const { return m_fragment_path; }

    void BindUniformBuffer(const std::string& block_name, UniformBufferObj* buffer);
    unsigned int GetUniformBlockIndex(const std::string& block_name) const;
 };
