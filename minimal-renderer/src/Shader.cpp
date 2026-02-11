#include <fstream>
#include <sstream>
#include <iostream>

#include "Shader.h"

Shader::Shader(const std::string& vertex_path, const std::string& fragment_path)
    : m_vertex_path(vertex_path), m_fragment_path(fragment_path)
{
    // retrieve the vertex/fragment source code from file path
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

        // read files' buffer contents into streams
        v_shader_stream << v_shader_file.rdbuf();
        f_shader_stream << f_shader_file.rdbuf();
        // close file handlers
        v_shader_file.close();
        f_shader_file.close();
        // convert stream into string
        vertex_code = v_shader_stream.str();
        fragment_code = f_shader_stream.str();
    }
    catch (const std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << m_vertex_path << '\n';
    }
    const char* v_shader_code = vertex_code.c_str();
    const char* f_shader_code = fragment_code.c_str();

    // compile shaders
    GLuint vertex, fragment;
    GLint success;
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

Shader::Shader(const std::string &vertex_path, const std::string &fragment_path, const std::string &geometry_path)
    : m_vertex_path(vertex_path), m_fragment_path(fragment_path), m_geometry_path(geometry_path)
{
    // retrieve the source code from the file path
    std::string vertex_code, fragment_code, geometry_code;
    std::ifstream v_shader_file, f_shader_file, g_shader_file;

    // ensure ifstream objects can throw exceptions
    v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    g_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // open file
        v_shader_file.open(m_vertex_path);
        f_shader_file.open(m_fragment_path);
        g_shader_file.open(m_geometry_path);
        std::stringstream v_shader_stream, f_shader_stream, g_shader_stream;

        // read files' buffer contents into streams
        v_shader_stream << v_shader_file.rdbuf();
        f_shader_stream << f_shader_file.rdbuf();
        g_shader_stream << g_shader_file.rdbuf();
        // close file handlers
        v_shader_file.close();
        f_shader_file.close();
        g_shader_file.close();
        // convert stream into string
        vertex_code = v_shader_stream.str();
        fragment_code = f_shader_stream.str();
        geometry_code = g_shader_stream.str();
    }
    catch (const std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << m_vertex_path << '\n';
    }
    const char* v_shader_code = vertex_code.c_str();
    const char* f_shader_code = fragment_code.c_str();
    const char* g_shader_code = geometry_code.c_str();

    // compile shaders
    GLuint vertex, fragment, geometry;
    GLint success;
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

    // geometry shader
    GLCall(geometry = glCreateShader(GL_GEOMETRY_SHADER));
    GLCall(glShaderSource(geometry, 1, &g_shader_code, nullptr));
    GLCall(glCompileShader(geometry));
    // print errors if any
    GLCall(glGetShaderiv(geometry, GL_COMPILE_STATUS, &success));
    if (!success)
    {
        GLCall(glGetShaderInfoLog(geometry, 512, nullptr, info_log));
        std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << info_log << std::endl;
    };

    // shader program
    GLCall(m_renderer_id = glCreateProgram());
    GLCall(glAttachShader(m_renderer_id, vertex));
    GLCall(glAttachShader(m_renderer_id, fragment));
    GLCall(glAttachShader(m_renderer_id, geometry));
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
    GLCall(glDeleteShader(geometry));
}

Shader::~Shader()
{
    if (m_renderer_id != 0)
    {
        GLCall(glDeleteProgram(m_renderer_id));
    }
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
    if (auto it = m_uniform_cache.find(name); it != m_uniform_cache.end())
    {
        location = it->second;
        return location != -1;
    }

    GLCall(location = glGetUniformLocation(m_renderer_id, name.c_str()));
    m_uniform_cache[name] = location;

    if (location == -1)
    {
        std::cout << "Warning: Uniform '" << name << "' doesn't exist!\n";
        return false;
    }
    return true;
}

void Shader::SetUniform(const std::string& name, bool value)
{
    if (int location; GetUniformLocation(name, location))
    {
        GLCall(glUniform1i(location, static_cast<int>(value)));
    }
}

void Shader::SetUniform(const std::string& name, int value)
{
    if (int location; GetUniformLocation(name, location))
    {
        GLCall(glUniform1i(location, value));
    }
}

void Shader::SetUniform(const std::string& name, float value)
{
    if (int location; GetUniformLocation(name, location))
    {
        GLCall(glUniform1f(location, value));
    }
}

void Shader::SetUniform(const std::string& name, float x, float y, float z)
{
    if (int location; GetUniformLocation(name, location))
    {
        GLCall(glUniform3f(location, x, y, z));
    }
}

void Shader::SetUniform(const std::string& name, float x, float y, float z, float w)
{
    if (int location; GetUniformLocation(name, location))
    {
        GLCall(glUniform4f(location, x, y, z, w));
    }
}

void Shader::SetUniform(const std::string& name, const glm::mat4& value)
{
    if (int location; GetUniformLocation(name, location))
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

GLuint Shader::GetUniformBlockIndex(const std::string& block_name) const
{
    GLCall(GLuint idx = glGetUniformBlockIndex(m_renderer_id, block_name.c_str()));
    return idx;
}

void Shader::BindUniformBuffer(const std::string& block_name, UniformBufferObj* buffer)
{
    GLuint block_idx = GetUniformBlockIndex(block_name);

    // Check if the block exists
    if (block_idx == GL_INVALID_INDEX)
    {
        std::cout << "Warning: Uniform block '" << block_name << "' not found in shader!\n";
        return;
    }

    auto binding_point = static_cast<GLuint>(u_uniform_buffers.size());
    u_uniform_buffers.emplace_back(block_name, buffer);

    // Connect shader block to binding point
    GLCall(glUniformBlockBinding(m_renderer_id, block_idx, binding_point));

    // Bind the actual buffer to that binding point
    GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, buffer->GetID()));
}