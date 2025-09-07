#include <fstream>
#include <sstream>
#include <iostream>

#include "Shader.h"
#include "Renderer.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	// retrive the vertex/fragment source code from file path
	std::string vertexCode, fragmentCode;
	std::ifstream vShaderFile, fShaderFile;
	// ensure ifstream objects can throw exceptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open file
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// compile shaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// vertex shader
	GLCall(vertex = glCreateShader(GL_VERTEX_SHADER));
	GLCall(glShaderSource(vertex, 1, &vShaderCode, nullptr));
	GLCall(glCompileShader(vertex));
	// print errors if any
	GLCall(glGetShaderiv(vertex, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		GLCall(glGetShaderInfoLog(vertex, 512, nullptr, infoLog));
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
	
	// fragment shader
	GLCall(fragment = glCreateShader(GL_FRAGMENT_SHADER));
	GLCall(glShaderSource(fragment, 1, &fShaderCode, nullptr));
	GLCall(glCompileShader(fragment));
	// print errors if any
	GLCall(glGetShaderiv(fragment, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		GLCall(glGetShaderInfoLog(fragment, 512, nullptr, infoLog));
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	// shader program
	GLCall(m_RendererID = glCreateProgram());
	GLCall(glAttachShader(m_RendererID, vertex));
	GLCall(glAttachShader(m_RendererID, fragment));
	GLCall(glLinkProgram(m_RendererID));
	// print errors if any
	GLCall(glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success));
	if (!success)
	{
		GLCall(glGetProgramInfoLog(m_RendererID, 512, nullptr, infoLog));
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// delete the shaders as they're linked into the program now and no longer necessary
	GLCall(glDeleteShader(vertex));
	GLCall(glDeleteShader(fragment));
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

bool Shader::GetUniformLocation(const std::string& name, int& location)
{
	GLCall(location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
	{
		std::cout << "Warning: Uniform '" << name << "' doesn't exist!" << std::endl;
		return false;
	}
	return true;
}

void Shader::SetBool(const std::string& name, bool value)
{
	int location;
	if (GetUniformLocation(name, location))
	{
		GLCall(glUniform1i(location, int(value)));
	}
}

void Shader::SetInt(const std::string& name, int value)
{
	int location;
	if (GetUniformLocation(name, location))
	{
		GLCall(glUniform1i(location, value));
	}
}

void Shader::SetFloat(const std::string& name, float value)
{
	int location;
	if (GetUniformLocation(name, location))
	{
		GLCall(glUniform1f(location, value));
	}
}

void Shader::SetVec4(const std::string& name, const float x, const float y, const float z, const float w)
{
	int location;
	if (GetUniformLocation(name, location))
	{
		glUniform4f(location, x, y, z, w);
	}
}

void Shader::SetMat4(const std::string& name, glm::mat4 value)
{
	int location;
	if (GetUniformLocation(name, location))
	{
		GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)));
	}
}
