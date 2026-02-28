#pragma once
#include <vector>
#include "UniformBufferObject.h"
#include "../Scene.h"

class Model;

class SceneAdvancedGLSL : public Scene
{
private:
    std::vector<Model*> m_objects;
    UniformBufferObj* m_uniform_buffer = nullptr;


public:
    SceneAdvancedGLSL();
    ~SceneAdvancedGLSL();

    static std::string StaticName() { return "21_AdvancedGLSL"; }

    void OnUpdate(double delta_time) override;
    void OnRender() override;
};
