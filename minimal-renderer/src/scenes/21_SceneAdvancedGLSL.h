#pragma once
#include <memory>
#include <vector>

#include "Renderer.h"
#include "UniformBufferObject.h"
#include "Scene.h"

namespace scene {
    class SceneAdvancedGLSL : public Scene
    {
    private:
        std::vector<Model*> m_objects;
        UniformBufferObj* m_uniform_buffer = nullptr;


    public:
        SceneAdvancedGLSL();
        ~SceneAdvancedGLSL();

        void OnUpdate(double delta_time) override;
        void OnRender() override;
    };
}