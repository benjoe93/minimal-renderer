#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "Shader.h"
#include "Texture2D.h"

class Material
{
private:
    std::shared_ptr<Shader> m_shader;
    std::vector<std::pair<std::string, std::shared_ptr<Texture>>> m_textures;

    std::unordered_map<std::string, float> u_floats;
    std::unordered_map<std::string, glm::vec3> u_vec3;
    std::unordered_map<std::string, glm::mat4> u_mat4;

    std::string m_vertex_path;
    std::string m_fragment_path;

public:
    Material(std::string vertex_path, std::string fragment_path);
    ~Material();

    void AddTexture(std::shared_ptr<Texture> texture);
    void AddTexture2D(std::string path, std::string sampler_name, bool is_flipped);

    void SetUniform(const std::string type, float value);
    void SetUniform(const std::string type, const glm::vec3& value);
    void SetUniform(const std::string type, const glm::mat4& value);

    void Bind();
    void Unbind();

    Shader& GetShader() const { return *m_shader; }
    const std::string& GetVertexPath() const { return m_vertex_path; }
    const std::string& GetFragmentPath() const { return m_fragment_path; }
};