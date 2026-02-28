#include "AppState.h"
#include "Camera.h"

AppState& AppState::Get() {
    static AppState s_instance;
    return s_instance;
}

void AppState::RegisterCamera(Camera &cam) {
    m_cameras.try_emplace(cam.GetId(), &cam);
}

void AppState::SetActiveCamera(int cam_id) {
    m_active_camera = cam_id;
}

Camera& AppState::GetActiveCamera() {
    auto it = m_cameras.find(m_active_camera);
    if (it == m_cameras.end())
    {
        throw std::runtime_error("Active camera not found!");
    }
    return *it->second;
}

void AppState::SetCursorState(CursorState new_state) {
    switch (new_state) {
        case CursorState::ENABLED:
            m_cursor_enabled = true;
            break;
        case CursorState::DISABLED:
            m_cursor_enabled = false;
            break;
    }
}

void AppState::SetCursorLastPosition(int x, int y) {
    m_cursor_last_pos = {x, y};
}

void AppState::SetScreenSize(GLuint width, GLuint height) {
    m_scr_width = width;
    m_scr_height = height;
}
