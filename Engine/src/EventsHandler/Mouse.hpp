

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <functional>
#include <utility>

#include "../Logger.hpp"

namespace EventsHandler
{
    class Mouse
    {
    public:
        Mouse();

        void Process(const int x, const int y);
        inline float GetX() const { return yaw; };
        inline float GetY() const { return pitch; };

    private:
        bool firstMove = true;
        float x;
        float y;

        float xDelta;
        float yDelta;

        float yaw = 0.0f;
        float pitch = -90.0f;
    };
};