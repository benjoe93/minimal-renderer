#pragma once
#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <string>
#include "Texture.h"


class Shader
{
private:
	unsigned int m_RendererID;
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();
	
	void Bind() const;
	void Unbind() const;

	// utility uniform functions
	int GetUniformLocation(const std::string& name);
	void SetUniform1b(const std::string& name, bool value) const;
	void SetUniform1i(const std::string& name, int value) const;
	void SetUniform1f(const std::string& name, float value) const;
};
