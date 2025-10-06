#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera;
class Shader;
class IndexBuffer;
class VertexArray;
class Model;

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

    unsigned int active_camera;
    std::unordered_map<unsigned int, std::shared_ptr<Camera>> cameras;
};

class Renderer
{
    private:
	    glm::vec4 m_background_color;
        bool m_use_depth_buffer;
        bool m_use_stencil_buffer;
        bool m_enable_wireframe;
        bool m_enable_face_culling;

        double m_delta_time;
        double m_last_frame;

    public:
        std::unique_ptr<AppState> state;

    public:
	    Renderer(const bool depth_testing = true, const bool stencil_testing = true, const bool wireframe = false, const bool face_culling = false);
	    ~Renderer();

	    void Clear() const;
	    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
        void Draw(Model& obj);

        void Tick(double current_time);

        void SetDepthTest(bool enabled);
        void SetFaceCulling(bool enabled);
        void SetStencilTest(bool enabled);
	    void SetWireframeRender(bool enabled);

        /* Specifies the comparison function for depth testing. For more info: https://docs.gl/gl3/glDepthFunc */
        void SetDepthFunction(GLenum function);
        /* Specifies the comparison function for stencil testing. For more info: https://docs.gl/gl3/glStencilFunc */
        void SetStencilFunction(GLenum function, GLint reference_value, GLuint bit_mask);
        /* Controls the writing of individual bits in the stencil planes. For more info: https://docs.gl/gl3/glStencilMask */
        void SetStencilMask(GLuint bit_mask);
        /* Sets front and back stencil test actions. For more info: https://docs.gl/gl3/glStencilOp
        - stencil_fail: operate when the stencil test fails
        - depth_fail:   operate when stencil passes but depth test fails
        - pass:         operate when both stencil and depth tests pass
        */
        void SetStencilOperation(GLenum stencil_fail, GLenum depth_fail, GLenum pass);

	    inline void SetBackgroundColor(glm::vec4 new_color) { m_background_color = new_color; }

    public:
        Camera& GetActiveCamera() const;
	    int GetMaxVertexAttribs() const;

        inline double GetDeltaTime() const { return m_delta_time; }
        inline float GetDeltaTimeFloat() const { return static_cast<float>(m_delta_time); }

        inline bool GetDepthTest() const { return m_use_depth_buffer; }
        inline bool GetWireframe() const { return m_enable_wireframe; }
        inline bool GetFaceCulling() const { return m_enable_face_culling; }
};