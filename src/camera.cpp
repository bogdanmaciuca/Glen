#include "camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "mathdef.h"

Mat4 Camera::GetViewMatrix() {
    return glm::lookAt(m_pos, m_pos + m_front, s_up);
}

Vec3 Camera::GetPos() {
    return m_pos;
}

void Camera::MoveForward(float deltaTime) {
    m_pos += m_front * CAMERA_SPEED * deltaTime;
}

void Camera::MoveBackward(float deltaTime) {
    m_pos -= m_front * CAMERA_SPEED * deltaTime;
}

void Camera::MoveLeft(float deltaTime) {
    m_pos -= glm::normalize(glm::cross(m_front, s_up)) * CAMERA_SPEED * deltaTime;
}

void Camera::MoveRight(float deltaTime) {
    m_pos += glm::normalize(glm::cross(m_front, s_up)) * CAMERA_SPEED * deltaTime;
}

void Camera::ProcessMouse(float deltaX, float deltaY) {
    m_yaw += deltaX * CAMERA_SENSITIVITY;
    m_pitch -= deltaY * CAMERA_SENSITIVITY;
    m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);

    m_front.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
    m_front.y = glm::sin(glm::radians(m_pitch));
    m_front.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
    m_front = glm::normalize(m_front);
}

