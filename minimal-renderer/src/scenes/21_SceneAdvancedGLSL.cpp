#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "vendor/imgui/imgui.h"

#include "Camera.h"
#include "Texture.h"
#include "Material.h"
#include "Model.h"
#include "ResourceManager.h"

#include "21_SceneAdvancedGLSL.h"

#include "SceneRegistry.h"

SceneAdvancedGLSL::SceneAdvancedGLSL()
    : Scene(StaticName())
{
    // create the buffer
    m_uniform_buffer = new UniformBufferObj(2 * sizeof(glm::mat4));
    // define the range of the buffer that links to a uniform binding point
    m_uniform_buffer->SetRange(0, 0, 2 * sizeof(glm::mat4));

    // store the projection matrix (we only do this once now) (note: we're not using zoom anymore by changing the FoV)
    Camera& cam = AppState::Get().GetActiveCamera();
    glm::mat4 projection = glm::perspective(
        glm::radians(cam.GetFov()),
        static_cast<float>(AppState::Get().GetScreenWidth()) / static_cast<float>(AppState::Get().GetScreenHeight()),
        AppState::Get().GetNearPlane(),
        AppState::Get().GetFarPlane());
    m_uniform_buffer->Bind();
    m_uniform_buffer->StoreData(0, sizeof(glm::mat4), glm::value_ptr(projection));
    m_uniform_buffer->Unbind();

    // Resources
    std::string plane = "resources/models/plane.fbx";
    std::string box = "resources/models/box.fbx";

    std::string vert_path = "resources/shaders/03_AdvancedOpenGL/06_Advanced_GLSL/uniform_buffer_obj.vert";
    std::string frag_path = "resources/shaders/03_AdvancedOpenGL/06_Advanced_GLSL/uniform_buffer_obj.frag";

    // Texture2D* metal_tex = ResourceManager::Get().GetTexture2D("resources/textures/metal.png");
    // Texture2D* marble_tex = ResourceManager::Get().GetTexture2D("resources/textures/container.jpg");

    auto box_material = ResourceManager::Get().GetMaterial("box_material");
    if (!box_material) {
        box_material = ResourceManager::Get().CreateMaterial("box_material", vert_path, frag_path);
    }


    // Models
    auto box0 = new Model(box, Transform(glm::vec3(-0.75f,  0.75f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f)));
    auto box1 = new Model(box, Transform(glm::vec3( 0.75f,  0.75f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f)));
    auto box2 = new Model(box, Transform(glm::vec3(-0.75f, -0.75f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f)));
    auto box3 = new Model(box, Transform(glm::vec3( 0.75f, -0.75f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f)));
    box0->SetMaterialSlot(0, box_material);
    box1->SetMaterialSlot(0, box_material);
    box2->SetMaterialSlot(0, box_material);
    box3->SetMaterialSlot(0, box_material);

    m_objects.push_back(box0);
    m_objects.push_back(box1);
    m_objects.push_back(box2);
    m_objects.push_back(box3);

    // Material uniforms - set different colors for each box
    for (Material* mat : box0->GetAllMaterials()) {
        mat->SetUniform("color", glm::vec3(1.0f, 0.0f, 0.0f));
        mat->AddUniformBuffer("matrices", m_uniform_buffer);
    }

    for (Material* mat : box1->GetAllMaterials()) {
        mat->SetUniform("color", glm::vec3(0.0f, 1.0f, 0.0f));
        mat->AddUniformBuffer("matrices", m_uniform_buffer);
    }

    for (Material* mat : box2->GetAllMaterials()) {
        mat->SetUniform("color", glm::vec3(0.0f, 0.0f, 1.0f));
        mat->AddUniformBuffer("matrices", m_uniform_buffer);
    }

    for (Material* mat : box3->GetAllMaterials()) {
        mat->SetUniform("color", glm::vec3(1.0f, 1.0f, 0.0f));
        mat->AddUniformBuffer("matrices", m_uniform_buffer);
    }
}

SceneAdvancedGLSL::~SceneAdvancedGLSL()
{
    for (Model* model : m_objects) {
        delete model;
    }
    m_objects.clear();
}

void SceneAdvancedGLSL::OnUpdate(double delta_time)
{
    Renderer::Get().SetBackgroundColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    Camera& cam = AppState::Get().GetActiveCamera();

    glm::mat4 projection, ModelView, MVP, view;
    projection = glm::perspective(
        glm::radians(cam.GetFov()),
        static_cast<float>(AppState::Get().GetScreenWidth()) / static_cast<float>(AppState::Get().GetScreenHeight()),
        AppState::Get().GetNearPlane(),
        AppState::Get().GetFarPlane());
    view = cam.GetViewMatrix();

    // set the view matrix in the uniform block - we only have to do this once per loop iteration.
    m_uniform_buffer->Bind();
    m_uniform_buffer->StoreData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    m_uniform_buffer->Unbind();

    // objects
    for (auto& obj : m_objects)
    {
        ModelView = cam.GetViewMatrix() * obj->GetModelMatrix();
        MVP = projection * ModelView;

        for (Material* mat : obj->GetAllMaterials()) {
            mat->SetUniform("model", obj->GetModelMatrix());
        }
    }
}

void SceneAdvancedGLSL::OnRender()
{
    for (auto& obj : m_objects)
        Renderer::Get().Draw(*obj);
}

REGISTER_SCENE(SceneAdvancedGLSL);