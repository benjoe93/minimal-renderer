#pragma once
#include "Renderer.h"



class Camera
{
private:
    static constexpr float DEFAULT_SPEED = 2.5f;
    static constexpr float DEFAULT_FOV = 45.0f;

    unsigned int m_renderer_id;

    float m_cam_speed = DEFAULT_SPEED;
    float m_fov = DEFAULT_FOV;
    float m_pitch = 0.0f, m_yaw = -90.0f, m_roll = 0.0f;
    glm::vec3 m_cam_position{}, m_cam_direction{}, m_cam_up{};

public:
    Camera(const unsigned int id, const glm::vec3& position, const glm::vec3& front = glm::vec3(0.0f, 0.0f, -1.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));
    ~Camera() = default;

    glm::mat4 GetViewMatrix() const;

    void SetPosition(const glm::vec3& new_pos) { m_cam_position = new_pos; }
    void SetSpeed(const float new_speed) { m_cam_speed = new_speed; }
    void SetPitch(const float new_pitch);
    void SetYaw(const float new_yaw);
    void SetRoll(const float new_roll);
    void AddPitch(const float new_pitch);
    void AddYaw(const float new_yaw);
    void AddRoll(const float new_roll);
    void SetFov(const float new_fov) { m_fov = new_fov; }

    unsigned int GetId() const { return m_renderer_id; }
    glm::vec3 GetPosition() const { return m_cam_position; }
    glm::vec3 GetDirection() const { return m_cam_direction; }
    glm::vec3 GetUpVector() const { return m_cam_up; }
    float GetSpeed() const { return m_cam_speed; }

    float GetPitch() const { return m_pitch; }
    float GetYaw() const { return m_yaw; }
    float GetRoll() const { return m_roll; }

    float GetFov() const { return m_fov; }

    void MoveFwd(const double delta_time);
    void MoveBwd(const double delta_time);
    void MoveRight(const double delta_time);
    void MoveLeft(const double delta_time);
    void MoveDown(const double delta_time);
    void MoveUp(const double delta_time);
    void UpdateRotation();
};