#pragma once

#include <memory>
#include <map>
#include <string>
#include <vector>
#include <functional>

#include "Scene.h"

class SceneRegistry {
public:
    using SceneFactory = std::function<std::unique_ptr<Scene>()>;

private:
    SceneRegistry() = default;
    ~SceneRegistry() = default;

    SceneRegistry(const SceneRegistry&) = delete;
    SceneRegistry& operator=(const SceneRegistry&) = delete;

    std::map<std::string, SceneFactory> m_scenes;
public:
    static SceneRegistry& Get();

    void Register(const std::string& scene_name, SceneFactory factory);
    std::unique_ptr<Scene> CreateScene(const std::string& scene_name) const;
    std::vector<std::string> GetSceneNames() const;
};

// compile-time constraint, that makes sure that `T` derived from the `Scene` class
template<typename T>
concept SceneType =
    std::derived_from<T, Scene> &&
    requires { {T::StaticName() } -> std::convertible_to<std::string>; };

// This function only exists if `T` satisfies `SceneType`
template<SceneType T>
void RegisterScene() {
    SceneRegistry::Get().Register(
        T::StaticName(),
        [] { return std::make_unique<T>(); }
    );
}

// Macro to register a scene class
#define REGISTER_SCENE(T)                    \
    namespace {                              \
        const bool reg_##T = []() {          \
            RegisterScene<T>();              \
            return true;                     \
        }();                                 \
    }