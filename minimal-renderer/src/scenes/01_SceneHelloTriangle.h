#pragma once
#include "Scene.h"

class Renderer;
class VertexArray;
class VertexBuffer;
class IndexBuffer;

namespace scene {
    class SceneHelloTriangle : public Scene
    {
    private:
        unsigned int vertex_shader = 0;
        unsigned int fragment_shader = 0;
        unsigned int shader_program = 0;
        unsigned int VBO = 0;
        unsigned int VAO = 0;


    public:
        SceneHelloTriangle();

        void OnUpdate(double delta_time) override;
        void OnRender() override;
        void OnImGuiRender() override;
    };
}
