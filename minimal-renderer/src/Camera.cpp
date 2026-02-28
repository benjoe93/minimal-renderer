#include "Camera.h"

Camera::Camera(const unsigned int id, const glm::vec3& position, const glm::vec3& front, const glm::vec3& up)
    : m_renderer_id(id),
    m_cam_position(position),
    m_cam_direction(front),
    m_cam_up(up)
{}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(m_cam_position, m_cam_position + m_cam_direction, m_cam_up);
}

void Camera::SetPitch(const float new_pitch)
{
    m_pitch = new_pitch;
    UpdateRotation();
}

void Camera::SetYaw(const float new_yaw)
{
    m_yaw = new_yaw;
    UpdateRotation();
}

void Camera::SetRoll(const float new_roll)
{
    m_roll = new_roll;
    UpdateRotation();
}

void Camera::AddPitch(const float new_pitch)
{
    m_pitch += new_pitch;
    UpdateRotation();
}

void Camera::AddYaw(const float new_yaw)
{
    m_yaw += new_yaw;
    UpdateRotation();
}

void Camera::AddRoll(const float new_roll)
{
    m_roll += new_roll;
    UpdateRotation();
}

void Camera::MoveFwd(const double delta_time)
{
    const float velocity = static_cast<float>(delta_time * m_cam_speed);
    m_cam_position += velocity * m_cam_direction;
}

void Camera::MoveBwd(const double delta_time)
{
    const float velocity = static_cast<float>(delta_time * m_cam_speed);
    m_cam_position -= velocity * m_cam_direction;
}

void Camera::MoveRight(const double delta_time)
{
    const glm::vec3 right = glm::normalize(glm::cross(m_cam_direction, m_cam_up));
    const float velocity = static_cast<float>(delta_time * m_cam_speed);
    m_cam_position += velocity * right;
}

void Camera::MoveLeft(const double delta_time)
{
    const glm::vec3 right = glm::normalize(glm::cross(m_cam_direction, m_cam_up));
    const float velocity = static_cast<float>(delta_time * m_cam_speed);
    m_cam_position -= velocity * right;
}

void Camera::MoveDown(const double delta_time)
{
    const float velocity = static_cast<float>(delta_time * m_cam_speed);
    m_cam_position -= velocity * glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::MoveUp(const double delta_time)
{
    const float velocity = static_cast<float>(delta_time * m_cam_speed);
    m_cam_position += velocity * glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::UpdateRotation()
{
    const glm::vec3 temp_dir{
        cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)),  // x
        sin(glm::radians(m_pitch)),                             // y
        sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch))   // z
    };
    m_cam_direction = glm::normalize(temp_dir);
}

void Camera::Reset() {
    m_fov = DEFAULT_FOV;
    m_cam_position = {0.0f, 0.0f, 3.0f};
    m_cam_direction = {0.0f, 0.0f, -1.0f};
    m_cam_up = {0.0f, 1.0f, 0.0f};
    m_pitch = 0.0f;
    m_yaw = -90.0f;
    m_roll = 0.0f;
    UpdateRotation();
}
