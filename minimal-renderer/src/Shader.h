#pragma once
#include <string>

#include "Renderer.h"
#include "Texture.h"


class Shader
{
private:
    unsigned int m_renderer_id;
    std::string& m_vertex_path;
    std::string& m_fragment_path;

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
};
