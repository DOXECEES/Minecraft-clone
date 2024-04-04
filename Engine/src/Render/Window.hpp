#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "../Logger.hpp"
#include "../EventsHandler/Keyboard.hpp"
#include "../EventsHandler/Mouse.hpp"
#include "../EventsHandler/Input.hpp"

namespace Renderer
{
    class Window
    {

    public:
        Window();
        Window(uint32_t width, uint32_t height, const char *title);
        Window(const Window &other) = delete;
        ~Window() = default;

        bool Render();
        GLFWwindow *GetWindow() const { return window; }

        bool IsKeyPressed(const uint16_t keyCode) const { return input->kbr->isPressed(keyCode); }
        

    public:
        // std::unique_ptr<EventsHandler::Keyboard> kbr;
        //  std::unique_ptr<EventsHandler::Mouse> mouse;
        std::unique_ptr<EventsHandler::Input> input;

    private:
        GLFWwindow *window;
        uint32_t width;
        uint32_t height;
        const char *title;
    };
}
