#pragma once
#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	bool GetUniformLocation(const std::string& name, int& location);
	void SetBool(const std::string& name, bool value);
	void SetInt(const std::string& name, int value);
	void SetFloat(const std::string& name, float value);
	void SetVec4(const std::string& name, const float x, const float y, const float z, const float w);
	void SetMat4(const std::string& name, glm::mat4 value);

};
