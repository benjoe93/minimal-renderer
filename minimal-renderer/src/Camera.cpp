#include "Camera.h"

Camera::Camera(const unsigned int id, const glm::vec3 Position, const glm::vec3 Front, const glm::vec3 Up)
	: m_renderer_id(id),
	m_pitch(0.0f),
	m_yaw(-90.0f),
	m_roll(0.0f),
	m_cam_position(Position),
	m_cam_direction(Front),
	m_cam_up(Up)
{}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(m_cam_position, m_cam_position + m_cam_direction, m_cam_up);
}

void Camera::MoveFwd(const double delta_time)
{
	m_cam_position += static_cast<float>(delta_time * m_cam_speed) * m_cam_direction;
}

void Camera::MoveBwd(const double delta_time)
{
	m_cam_position -= static_cast<float>(delta_time * m_cam_speed) * m_cam_direction;
}

void Camera::MoveRight(const double delta_time)
{
	m_cam_position += static_cast<float>(delta_time * m_cam_speed) * glm::normalize(glm::cross(m_cam_direction, m_cam_up));
}

void Camera::MoveLeft(const double delta_time)
{
	m_cam_position -= static_cast<float>(delta_time * m_cam_speed) * glm::normalize(glm::cross(m_cam_direction, m_cam_up));
}

void Camera::MoveDown(const double delta_time)
{
	m_cam_position -= static_cast<float>(delta_time * m_cam_speed) * glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::MoveUp(const double delta_time)
{
	m_cam_position += static_cast<float>(delta_time * m_cam_speed) * glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::UpdateRotation(const double delta_time)
{
	glm::vec3 temp_dir{
		cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)),	// x
		sin(glm::radians(m_pitch)),								// y
		sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch))	// z
	};
	m_cam_direction = glm::normalize(temp_dir);
}
