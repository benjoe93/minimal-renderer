#pragma once
#include <vector>
#include "Scene.h"
#include "Model.h"

class Renderer;
class VertexArray;
class VertexBuffer;
class IndexBuffer;

    class _SceneEmpty : public Scene
    {
    private:
        std::vector<Model*> m_objects;

    public:
        _SceneEmpty();
        ~_SceneEmpty();

        static std::string StaticName() { return "00_SceneEmpty"; }

        void OnUpdate(double delta_time) override;
        void OnRender() override;
        void OnImGuiRender() override;
        void ConstructScene();
    };