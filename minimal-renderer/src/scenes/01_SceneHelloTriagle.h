#pragma once
#include <memory>
#include "LightDirectional.h"
#include "LightPoint.h"
#include "LightSpot.h"
#include "Scene.h"
#include "Material.h"
#include "Model.h"

class Renderer;
class VertexArray;
class VertexBuffer;
class IndexBuffer;

namespace scene {
    class SceneHelloTriagle : public Scene
    {
    private:
        unsigned int vertex_shader = 0;
        unsigned int fragment_shader = 0;
        unsigned int shader_program = 0;
        unsigned int VBO = 0;
        unsigned int VAO = 0;


    public:
        SceneHelloTriagle(Renderer& in_renderer);

        void OnUpdate(double delta_time) override;
        void OnRender() override;
        void OnImGuiRender() override;
    };
}