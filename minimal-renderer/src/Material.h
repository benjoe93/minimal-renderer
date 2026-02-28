#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "Shader.h"
#include "Texture.h"
#include "UniformBufferObject.h"

class Material
{
private:
    Shader* m_shader;
    std::vector<std::pair<std::string, Texture*>> m_textures;

    std::unordered_map<std::string, float> u_floats;
    std::unordered_map<std::string, glm::vec3> u_vec3;
    std::unordered_map<std::string, glm::mat4> u_mat4;

public:
    Material(const std::string& vertex_path, const std::string& fragment_path, const std::string& geometry_path = "");
    ~Material() = default;

    void AddTexture(const std::string& sampler_name, Texture* texture);
    void AddTexture2D(const std::string& path, const std::string& sampler_name, bool is_flipped = true);

    void SetUniform(const std::string& name, float value);
    void SetUniform(const std::string& name, const glm::vec3& value);
    void SetUniform(const std::string& name, const glm::mat4& value);

    void Bind();
    void Unbind();

    const Shader* GetShader() const { return m_shader; }
    const std::string& GetVertexPath() const { return m_shader->GetVertexPath(); }
    const std::string& GetFragmentPath() const { return m_shader->GetFragmentPath(); }

    void AddUniformBuffer(const std::string& block_name, UniformBufferObj* obj);
};