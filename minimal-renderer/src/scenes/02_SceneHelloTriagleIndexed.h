#pragma once
#include "Scene.h"

class Renderer;
class VertexArray;
class VertexBuffer;
class IndexBuffer;

namespace scene {
    class SceneHelloTriagleIdx : public Scene
    {
    private:
        unsigned int vertex_shader;
        unsigned int fragment_shader;
        unsigned int shader_program;
        unsigned int VBO;
        unsigned int VAO;
        unsigned int EBO;

    public:
        SceneHelloTriagleIdx(Renderer& in_renderer);

        void OnUpdate(double delta_time) override;
        void OnRender() override;
        void OnImGuiRender() override;
    };
}
