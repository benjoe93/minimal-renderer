#pragma once
#include <memory>
#include "Scene.h"

class Renderer;
class VertexArray;
class VertexBuffer;
class IndexBuffer;
class Shader;

namespace scene {
    class SceneTextures : public Scene
    {
    private:
        std::unique_ptr<Shader> shader;
        unsigned int VBO;
        unsigned int VAO;
        unsigned int EBO;

        unsigned int tex1, tex2;


    public:
        SceneTextures(Renderer& in_renderer);

        void OnUpdate(double delta_time) override;
        void OnRender() override;
        void OnImGuiRender() override;
    };
}
