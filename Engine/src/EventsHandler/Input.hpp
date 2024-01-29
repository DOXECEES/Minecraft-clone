
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Keyboard.hpp"
#include "Mouse.hpp"

namespace EventsHandler
{
    class Input
    {
    public:
        Input(GLFWwindow *window);

        // static void mouse_callback(GLFWwindow *window, double xpos, double ypos);

        // private:
        std::unique_ptr<Keyboard> kbr;
        std::unique_ptr<Mouse> mouse;
    };

};