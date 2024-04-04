// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "Mouse.hpp"

EventsHandler::Mouse::Mouse()
{
}

void EventsHandler::Mouse::Process(const int x, const int y)
{
    if (firstMove)
    {
        this->x = x;
        this->y = y;
        firstMove = false;
    }

    xDelta = this->x - x;
    yDelta = this->y - y;

    xDelta *= 0.1f;
    yDelta *= 0.1f;

    yaw += xDelta;
    pitch += yDelta;

    GLfloat xoffset = x - this->x;
    GLfloat yoffset = y - this->y;
    this->x = x;
    this->y = y;

    GLfloat sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
}