#pragma once
#include <memory>
#include "LightDirectional.h"
#include "LightPoint.h"
#include "LightSpot.h"
#include "Scene.h"
#include "Model.h"

class Renderer;
class VertexArray;
class VertexBuffer;
class IndexBuffer;
class Shader;

namespace scene {
    class SceneTransfrom : public Scene
    {
    private:
        std::unique_ptr<Shader> shader;

        std::unique_ptr<VertexArray>  vao;
        std::unique_ptr<VertexBuffer> vbo;
        std::unique_ptr<IndexBuffer>  ebo;

        std::shared_ptr<Texture2D> tex1, tex2;

        float location[2] = {0.0f, 0.0f};
        float angle = 45.0f;


    public:
        SceneTransfrom();

        void OnUpdate(double delta_time) override;
        void OnRender() override;
        void OnImGuiRender() override;
    };
}
