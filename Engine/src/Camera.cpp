// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "Camera.hpp"

Camera::Camera(glm::vec3 pos)
    : position(pos), front(0.0f, 0.0f, -1.0f), upVec(0.0f, 1.0f, 0.0f), direction(position + front)
{
    view = glm::lookAt(position, direction, upVec);
}

void Camera::SetPosition(glm::vec3 pos)
{
    position = pos;
}

void Camera::Update(const glm::vec3 pos, const float yaw, const float pitch)
{
    glm::vec3 _front;
    _front.x = -cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    _front.y = sin(glm::radians(pitch));
    _front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(_front);
    position = pos;
    direction = position + front;
    view = glm::lookAt(position, direction, upVec);
}
