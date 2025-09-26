#include "Material.h"

Material::Material(const char* vertex_path, const char* fragment_path)
{
	m_shader = std::make_unique<Shader>(vertex_path, fragment_path);
}

Material::~Material()
{
}

void Material::AddTexture(std::string type, const char* path, bool is_rgba)
{
		m_textures.push_back({
				type,
				std::make_unique<Texture>(path, is_rgba ? GL_RGBA : GL_RGB) 
		});
}

void Material::SetUniformFloat(std::string type, float value)
{
	u_floats[type] = value;
}

void Material::SetUniformVec3(const std::string type, const glm::vec3& value)
{
	u_vec3[type] = value;
}

void Material::SetUniformMat4(const std::string type, const glm::mat4& value)
{
	u_mat4[type] = value;
}

void Material::Bind()
{
	// Bind shader
	m_shader->Bind();

	// Bind uniforms
	for (const auto& [name, value] : u_floats)
	{
		m_shader->SetFloat(name, value);
	}
	for (const auto& [name, value] : u_vec3)
	{
		m_shader->SetVec3(name, value[0], value[1], value[2]);
	}
	for (const auto& [name, value] : u_mat4) {
		m_shader->SetMat4(name, value);
	}

	// Bind textures
	for (int i = 0; i < m_textures.size(); i++)
	{
		m_textures[i].second->Bind(i);
		m_shader->SetInt(m_textures[i].first, i);
	}
}

void Material::Unbind()
{
	for (int i = 0; i < m_textures.size(); i++)
	{
		m_textures[i].second->Unbind();
	}
}
