#pragma once
#include "mathdef.h"

constexpr float CAMERA_SPEED       = 0.002f;
constexpr float CAMERA_SENSITIVITY = 0.30f;

class Camera {
public:
    Mat4 GetViewMatrix();
    Vec3 GetPos();
    void MoveForward(float deltaTime);
    void MoveBackward(float deltaTime);
    void MoveLeft(float deltaTime);
    void MoveRight(float deltaTime);
    void ProcessMouse(float deltaX, float deltaY);
private:
    Vec3  m_pos   = Vec3(0.0f);
    float m_yaw   = 0.0f; // Degrees
    float m_pitch = 0.0f;
    Vec3  m_front = Vec3(0.0f);
    Vec3  s_up    = Vec3(0, 1, 0);
};


