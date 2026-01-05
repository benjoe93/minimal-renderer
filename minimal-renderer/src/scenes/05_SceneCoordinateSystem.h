#pragma once
#include <memory>
#include "Scene.h"

class Renderer;
class VertexArray;
class VertexBuffer;
class IndexBuffer;
class Shader;

namespace scene {
    class SceneCoordinateSystem: public Scene
    {
    private:
        std::unique_ptr<Shader> shader;

        std::unique_ptr<VertexArray>  vao;
        std::unique_ptr<VertexBuffer> vbo;
        std::unique_ptr<IndexBuffer>  ebo;

        std::shared_ptr<Texture2D> tex1, tex2;

        float location[3] = {0.0f, 0.0f, 0.0f};
        float rotation[3] = {0.0f, 0.0f, 0.0f};

    public:
        SceneCoordinateSystem();

        void OnUpdate(double delta_time) override;
        void OnRender() override;
        void OnImGuiRender() override;
    };
}
