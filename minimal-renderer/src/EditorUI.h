#pragma once
#include <string>


struct GLFWwindow;

class EditorUI {
private:
    std::string m_requested_scene;

public:
    // ImGUI initialization
    void Init(GLFWwindow* window);
    // ImGUI cleanup
    void Shutdown();

    // Start ImGUI Frame
    void BeginFrame();
    // Draw menubar and docking space
    void Render();
    // End ImGUI Frame
    void EndFrame();

    //Returns requested scene name
    const std::string& GetRequestedScene() const { return m_requested_scene; }
    // Clears the request after Application handles it
    void ClearSceneRequest() { m_requested_scene = ""; }
};
