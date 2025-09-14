#pragma once

#include <iostream>
#include <memory>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera;
class Shader;
class IndexBuffer;
class VertexArray;

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

struct AppState
{
    unsigned int scr_width = 1280;
    unsigned int scr_height = 720;

    bool cursor_disabled = false;

    float last_x = 640;
    float last_y = 360;

    Camera* active_camera = nullptr;
};

class Renderer
{
    private:
	    glm::vec4 m_background_color;
        bool m_use_depth_buffer;
        bool m_enable_wireframe;
        bool m_enable_face_culling;

        double m_delta_time;
        double m_last_frame;

    public:
        std::unique_ptr<AppState> state;

    public:
	    Renderer(const bool depth_testing = true, const bool wireframe = false, const bool face_culling = false);
	    ~Renderer();

	    void Clear();
	    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

	    int GetMaxVertexAttribs() const;

	    inline void SetBackgroundColor(glm::vec4 new_color) { m_background_color = new_color; }
        inline void SetDepthTest(bool enabled) { m_use_depth_buffer = enabled; }
        inline void Setwireframe(bool enabled) { m_enable_wireframe = enabled; }
        inline void Setface_culling(bool enabled) { m_enable_face_culling = enabled; }
        inline double GetDeltaTime() const { return m_delta_time; }

        void Tick(double current_time);


    private:
        void ToggleDepthTest() const;
	    void TogglewireframeRender() const;
        void Toggleface_culling() const;
};