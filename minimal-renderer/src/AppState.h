#pragma once
#include <unordered_map>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Camera;

enum class CursorState {
    ENABLED = 0,
    DISABLED,
};

class AppState {
private:
    GLuint m_scr_width = 1280;
    GLuint m_scr_height = 720;

    float m_near_plane = 0.1f;
    float m_far_plane  = 100.0f;

    bool m_cursor_enabled = true;

    glm::vec2 m_cursor_last_pos = {640.0f, 360.0f};

    GLuint m_active_camera = 0;
    std::unordered_map<GLuint, Camera*> m_cameras;

private:
    AppState() = default;
    ~AppState() = default;
    AppState(const AppState &) = delete;
    AppState& operator = (const AppState &) = delete;

public:
    static AppState& Get();
    int GetScreenWidth() const { return m_scr_width; }
    int GetScreenHeight() const { return m_scr_height; }

    void RegisterCamera(Camera& cam);
    void SetActiveCamera(int cam_id);
    Camera& GetActiveCamera();

    void SetCursorState(CursorState new_state);
    bool GetCursorEnabled() const { return m_cursor_enabled; }

    void SetCursorLastPosition(int x, int y);
    const glm::vec2& GetCursorLastPosition() const { return m_cursor_last_pos; };

    float GetNearPlane() const { return m_near_plane; }
    float GetFarPlane() const { return m_far_plane; }

    void SetScreenSize(GLuint width, GLuint height);


};
