#pragma once

#include <iostream>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera;
class Material;
class Model;
class VertexArray;
class IndexBuffer;
class Shader;

#pragma region OpenGLErrorHandling
namespace GLDebug
    {
        inline void ClearError()
        {
            while (glGetError() != GL_NO_ERROR);
        }

        inline bool LogCall(const char* function, const char* file, int line)
        {
            bool success = true;
            while (GLenum error = glGetError())
            {
                std::cerr << "[OpenGL Error] (0x" << std::hex << error << std::dec
                          << "): " << function << " " << file << ":" << line << '\n';
                success = false;
            }
            return success;
        }
    }

    #ifdef NDEBUG
    #define GLCall(x) x
    #else
    #define ASSERT(x) if (!(x)) __debugbreak();
    #define GLCall(x) GLDebug::ClearError(); x; ASSERT(GLDebug::LogCall(#x, __FILE__, __LINE__))
    #endif
#pragma endregion

#pragma region Enums
    enum class TestingFunc
    {
        ALWAYS      = GL_ALWAYS,    // The depth test always passes.
        NEVER       = GL_NEVER,     // The depth test never passes.
        LESS        = GL_LESS,      // Passes if the fragment's depth value is less than the stored depth value.
        EQUAL       = GL_EQUAL,     // Passes if the fragment's depth value is equal to the stored depth value.
        LEQUAL      = GL_LEQUAL,    // Passes if the fragment's depth value is less than or equal to the stored depth value.
        GREATER     = GL_GREATER,   // Passes if the fragment's depth value is greater than the stored depth value.
        NOTEQUAL    = GL_NOTEQUAL,  // Passes if the fragment's depth value is not equal to the stored depth value.
        GEQUAL      = GL_GEQUAL     // Passes if the fragment's depth value is greater than or equal to the stored depth value.
    };
    enum class StencilOp
    {
        KEEP        = GL_KEEP,      // The currently stored stencil value is kept.
        ZERO        = GL_ZERO,      // The stencil value is set to 0.
        REPLACE     = GL_REPLACE,   // The stencil value is replaced with the reference value set with glStencilFunc.
        INCR        = GL_INCR,      // The stencil value is increased by 1 if it is lower than the maximum value.
        INCR_WRAP   = GL_INCR_WRAP, // Same as GL_INCR, but wraps it back to 0 as soon as the maximum value is exceeded.
        DECR        = GL_DECR,      // The stencil value is decreased by 1 if it is higher than the minimum value.
        DECR_WRAP   = GL_DECR_WRAP, // Same as GL_DECR, but wraps it to the maximum value if it ends up lower than 0.
        INVE        = GL_INVERT     // Bitwise inverts the current stencil buffer value.
    };
    enum class BlendFunc
    {
        ZERO                       = GL_ZERO,                       // Factor is equal to 0
        ONE                        = GL_ONE,                        // Factor is equal to 1
        SRC_COLOR                  = GL_SRC_COLOR,                  // Factor is equal to the SOURCE color vector
        ONE_MINUS_SRC_COLOR        = GL_ONE_MINUS_SRC_COLOR,        // Factor is eqaul to 1 - SOURCE color vector
        DST_COLOR                  = GL_DST_COLOR,                  // Factor is equal to the DESTINATION color vector
        ONE_MINUS_DST_COLOR        = GL_ONE_MINUS_DST_COLOR,        // Factor is equal to 1 - DESTINATION color vector
        SRC_ALPHA                  = GL_SRC_ALPHA,                  // Factor is eqaul to the ALPHA component of the SOURCE color vector
        ONE_MINUS_SRC_ALPHA        = GL_ONE_MINUS_SRC_ALPHA,        // Factor is equal to 1 - ALPHA component of the SOURCE color vector
        DST_ALPHA                  = GL_DST_ALPHA,                  // Factor is eqaul to the ALPHA component of the DESTINATION color vector
        ONE_MINUS_DST_ALPHA        = GL_ONE_MINUS_DST_ALPHA,        // Factor is equal to 1 - ALPHA component of the DESTINATION color vector
        CONSTANT_COLOR             = GL_CONSTANT_COLOR,             // Factor is equal to the CONSTANT color vector
        ONE_MINUS_CONSTANT_COLOR   = GL_ONE_MINUS_CONSTANT_COLOR,   // Factor is equal to 1 - CONSTANT color vector
        CONSTANT_ALPHA             = GL_CONSTANT_ALPHA,             // Factor is eqaul to the ALPHA component of the CONSTANT color vector
        ONE_MINUS_CONSTANT_ALPHA   = GL_ONE_MINUS_CONSTANT_ALPHA    // Factor is equal to 1 - ALPHA component of the CONSTANT color vector
    };
    enum class BlendEquation
    {
        ADD                 = GL_FUNC_ADD,              // result = SRC + DST
        SUBTRACT            = GL_FUNC_SUBTRACT,         // result = SRC - DST
        REVERSE_SUBTRACT    = GL_FUNC_REVERSE_SUBTRACT, // result = DST - SRC
        MIN                 = GL_MIN,                   // result = min(DST, SRC)
        MAX                 = GL_MAX                    // result = max(DST, SRC)
    };
    enum class FaceCullMode
    {
        BACK            = GL_BACK,          // Culls only back face
        FRONT           = GL_FRONT,         // Culls only front face
        FRONT_AND_BACK  = GL_FRONT_AND_BACK // Culls both the front and back faces
    };
    enum class FrontFace
    {
        CCW = GL_CCW,   // Counter-clock wise
        CW  = GL_CW     // Clock wise
    };
#pragma endregion

struct AppState
{
GLuint scr_width = 1280;
    GLuint scr_height = 720;

    float near_plane = 0.1f;
    float far_plane  = 100.0f;

    bool cursor_disabled = false;

    float last_x = 640.0f;
    float last_y = 360.0f;

    GLuint active_camera = 0;
    std::unordered_map<GLuint, Camera*> cameras;
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

        AppState m_state = AppState();

    private:
        Renderer();
        ~Renderer() = default;

    public:
        Renderer(const Renderer&) = delete; // delete copy constructor
        Renderer& operator=(const Renderer&) = delete;

        static Renderer& Get();

        /*Clear specified Render Targets
        Flags:
            - GL_COLOR_BUFFER_BIT
            - GL_DEPTH_BUFFER_BIT
            - GL_STENCIL_BUFFER_BIT
        */
        void Clear(GLbitfield bits = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT) const;
        void Tick(double current_time);

    #pragma region DrawFunctions
        /* Draw mesh using Vertex Array and Index Buffer class. Use when verts are shared using an Index buffer. */
        void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
        /* Draw mesh using Vertex Array class. Use when only have access to a list of vert data. */
        void Draw(const VertexArray& va, GLsizei count, Material& material) const;
        /* Draw mesh using Model class*/
        void Draw(Model& obj);
    #pragma endregion

    #pragma region RenderSettings
        // Render state
        void SetBlending(bool enabled);
        void SetDepthTest(bool enabled);
        void SetFaceCulling(bool enabled);
        void SetStencilTest(bool enabled);
        void SetWireframeRender(bool enabled);

        // Configuration
        /* Specifies the comparison function for depth testing. For more info: https://docs.gl/gl3/glDepthFunc */
        void SetDepthFunction(TestingFunc function);

        /* Specifies the comparison function for stencil testing. For more info: https://docs.gl/gl3/glStencilFunc */
        void SetStencilFunction(TestingFunc function, int reference_value, unsigned int bit_mask);

        /* Controls the writing of individual bits in the stencil planes. For more info: https://docs.gl/gl3/glStencilMask */
        void SetStencilMask(unsigned int bit_mask);

        /* Sets front and back stencil test actions. For more info: https://docs.gl/gl3/glStencilOp
        - stencil_fail: operate when the stencil test fails
        - depth_fail:   operate when stencil passes but depth test fails
        - pass:         operate when both stencil and depth tests pass
        */
        void SetStencilOperation(StencilOp stencil_fail, StencilOp depth_fail, StencilOp pass);

        /*Sets the source and destination factors*/
        void SetBlendFunction(BlendFunc src_factor, BlendFunc dst_factor);

        /*Sets the operation between source and destination component of the blending equation*/
        void SetBlendEquation(BlendEquation eq);

        void SetFaceCullingMode(FaceCullMode mode);
        void SetFrontFace(FrontFace mode);
        /* Set RenderTarget background color */
        void SetBackgroundColor(const glm::vec4& new_color);
        void SetScreenSize(GLuint width, GLuint height);
    #pragma endregion

        // Queries
        Camera& GetActiveCamera() const;
        GLint GetMaxVertexAttribs() const;
        AppState& GetState() { return m_state; }
        const AppState& GetState() const { return m_state; }
        GLuint GetScreenWidth() const { return m_state.scr_width; }
        GLuint GetScreenHeight() const { return m_state.scr_height; }

        double GetDeltaTime() const { return m_delta_time; }
        float GetDeltaTimeFloat() const { return static_cast<float>(m_delta_time); }

        bool IsBlending() const { return m_blending; }
        bool IsDepthTest() const { return m_depth_buffer; }
        bool IsStencilTest() const { return m_stencil_buffer; }
        bool IsWireframe() const { return m_wireframe; }
        bool IsFaceCulling() const { return m_face_culling; }
};
