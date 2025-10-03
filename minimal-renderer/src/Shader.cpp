#include <fstream>
#include <sstream>
#include <iostream>

#include "Shader.h"

Shader::Shader(const char* vertex_path, const char* fragment_path)
    : m_vertex_path(vertex_path), m_fragment_path(fragment_path)
{
	// retrive the vertex/fragment source code from file path
	std::string vertex_code, fragment_code;
	std::ifstream v_shader_file, f_shader_file;
	// ensure ifstream objects can throw exceptions
	v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open file
		v_shader_file.open(m_vertex_path);
		f_shader_file.open(m_fragment_path);
		std::stringstream v_shader_stream, f_shader_stream;

		// read file's buffer contents into streams
		v_shader_stream << v_shader_file.rdbuf();
		f_shader_stream << f_shader_file.rdbuf();
		// close file handlers
		v_shader_file.close();
		f_shader_file.close();
		// convert stream into string
		vertex_code = v_shader_stream.str();
		fragment_code = f_shader_stream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << m_vertex_path << std::endl;
	}
	const char* v_shader_code = vertex_code.c_str();
	const char* f_shader_code = fragment_code.c_str();

	// compile shaders
	unsigned int vertex, fragment;
	int success;
	char info_log[512];

	// vertex shader
	GLCall(vertex = glCreateShader(GL_VERTEX_SHADER));
	GLCall(glShaderSource(vertex, 1, &v_shader_code, nullptr));
	GLCall(glCompileShader(vertex));
	// print errors if any
	GLCall(glGetShaderiv(vertex, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		GLCall(glGetShaderInfoLog(vertex, 512, nullptr, info_log));
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
	};
	
	// fragment shader
	GLCall(fragment = glCreateShader(GL_FRAGMENT_SHADER));
	GLCall(glShaderSource(fragment, 1, &f_shader_code, nullptr));
	GLCall(glCompileShader(fragment));
	// print errors if any
	GLCall(glGetShaderiv(fragment, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		GLCall(glGetShaderInfoLog(fragment, 512, nullptr, info_log));
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
	};

	// shader program
	GLCall(m_renderer_id = glCreateProgram());
	GLCall(glAttachShader(m_renderer_id, vertex));
	GLCall(glAttachShader(m_renderer_id, fragment));
	GLCall(glLinkProgram(m_renderer_id));
	// print errors if any
	GLCall(glGetProgramiv(m_renderer_id, GL_LINK_STATUS, &success));
	if (!success)
	{
		GLCall(glGetProgramInfoLog(m_renderer_id, 512, nullptr, info_log));
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
	}
	// delete the shaders as they're linked into the program now and no longer necessary
	GLCall(glDeleteShader(vertex));
	GLCall(glDeleteShader(fragment));
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_renderer_id));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_renderer_id));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

bool Shader::GetUniformLocation(const std::string& name, int& location) const
{
	GLCall(location = glGetUniformLocation(m_renderer_id, name.c_str()));
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

void Shader::SetVec3(const std::string& name, const float x, const float y, const float z)
{
	int location;
	if (GetUniformLocation(name, location))
	{
		glUniform3f(location, x, y, z);
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

void Shader::PrintShaderFiles() const
{
    std::cout << "Shader ID: " << m_renderer_id << std::endl;
    std::cout << "Vert: " << m_vertex_path << std::endl;
    std::cout << "Frag: " << m_fragment_path << std::endl << std::endl;
}
