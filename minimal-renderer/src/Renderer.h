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

enum BlendFunc
{
    ZERO = 0,
    ONE,
    SRC_COLOR,
    ONE_MINUS_SRC_COLOR,
    DST_COLOR,
    ONE_MINUS_DST_COLOR,
    SRC_ALPHA,
    ONE_MINUS_SRC_ALPHA,
    DST_ALPHA,
    ONE_MINUS_DST_ALPHA,
    CONSTANT_COLOR,
    ONE_MINUS_CONSTANT_COLOR,
    CONSTANT_ALPHA,
    ONE_MINUS_CONSTANT_ALPHA
};
enum BlendEquation
{
    ADD = 0,
    SUBTRACT,
    REVERSE_SUBTRACT,
    MIN,
    MAX
};
enum FaceCullMode
{
    BACK = 0,       // Culls only back face
    FRONT,          // Culls only front face
    FRONT_AND_BACK  // Culls both the front and back faces
};
enum FrontFace
{
    CCW = 0,
    CW
};

struct AppState
{
    unsigned int scr_width = 1280;
    unsigned int scr_height = 720;

    float near_plane = 0.1f;
    float far_plane  = 100.0f;

    bool cursor_disabled = false;

    float last_x = 640;
    float last_y = 360;

    unsigned int active_camera = 0;
    std::unordered_map<unsigned int, std::shared_ptr<Camera>> cameras;
};

class Renderer
{
    private:
	    glm::vec4 m_background_color = { 0.1f, 0.1f, 0.1f, 1.0f };

        bool m_blending       = true;
        bool m_depth_buffer   = true;
        bool m_stencil_buffer = true;
        bool m_wireframe      = false;
        bool m_face_culling   = true;

        double m_delta_time = 0.0;
        double m_last_frame = 0.0;

    public:
        std::unique_ptr<AppState> state;

    public:
	    Renderer();
	    ~Renderer();

	    void Clear() const;
	    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
        void Draw(Model& obj);

        void Tick(double current_time);

        void SetBlending(bool enabled);
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

        /*Sets the source and destination factors*/
        void SetBlendFunction(BlendFunc src_factor, BlendFunc dst_factor);
        /*Sets the operation between source and destination component of the blending equation*/
        void SetBlendEquation(BlendEquation eq);

        void SetFaceCullingMode(FaceCullMode mode);
        void SetFrontFace(FrontFace mode);

	    inline void SetBackgroundColor(glm::vec4 new_color) { m_background_color = new_color; }
    private:
        GLenum GetBlendFunction(BlendFunc id) const;
        GLenum GetBlendEquation(BlendEquation id) const;
        GLenum GetCullFaceMode(FaceCullMode id) const;

    public:
        Camera& GetActiveCamera() const;
	    int GetMaxVertexAttribs() const;

        inline double GetDeltaTime() const { return m_delta_time; }
        inline float GetDeltaTimeFloat() const { return static_cast<float>(m_delta_time); }

        inline bool BlendingState() const { return m_blending; }
        inline bool DepthTestState() const { return m_depth_buffer; }
        inline bool StencilTestState() const { return m_stencil_buffer; }
        inline bool WireframeState() const { return m_wireframe; }
        inline bool FaceCullingState() const { return m_face_culling; }
};