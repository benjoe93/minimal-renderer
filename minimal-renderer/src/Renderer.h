#pragma once

#include <iostream>
#include <glad/glad.h>
#include <glm/vec4.hpp>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}


class Renderer
{
private:
	glm::vec4 m_BackgroundColor;
    bool m_UseDepthBuffer;
    bool m_EnableWireframe;
    bool m_EnableFaceCulling;

public:
	Renderer(const bool DepthTesting = true, const bool Wireframe = false, const bool FaceCulling = false);
	~Renderer();

	void Clear();
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

	int GetMaxVertexAttribs() const;

	inline void SetBackgroundColor(glm::vec4 new_color) { m_BackgroundColor = new_color; }
    inline void SetDepthTest(bool Enabled) { m_UseDepthBuffer = Enabled; }
    inline void SetWireframe(bool Enabled) { m_EnableWireframe = Enabled; }
    inline void SetFaceCulling(bool Enabled) { m_EnableFaceCulling = Enabled; }


private:
    void ToggleDepthTest() const;
	void ToggleWireframeRender() const;
    void ToggleFaceCulling() const;
};