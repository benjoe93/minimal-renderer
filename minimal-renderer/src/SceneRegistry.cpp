#include "SceneRegistry.h"

#include <iostream>
#include <ostream>
#include <ranges>


SceneRegistry& SceneRegistry::Get() {
    static SceneRegistry s_instance;
    return s_instance;
}

void SceneRegistry::Register(const std::string& scene_name, SceneFactory factory) {
    m_scenes.emplace(scene_name, std::move(factory));
}

std::unique_ptr<Scene> SceneRegistry::CreateScene(const std::string& scene_name) const {
    if (auto it = m_scenes.find(scene_name); it != m_scenes.end())
        return it->second();

    return nullptr;
}

std::vector<std::string> SceneRegistry::GetSceneNames() const {
    std::vector<std::string> scene_names;
    scene_names.reserve(m_scenes.size());

    if (m_scenes.empty()) {
        std::cout << "No scenes found in registry\n";
        return scene_names;
    }

    for (const auto &key: m_scenes | std::views::keys) {
        scene_names.push_back(key);
    }

    return scene_names;
}