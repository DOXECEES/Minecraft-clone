#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "../Logger.hpp"
#include "../EventsHandler/Keyboard.hpp"

namespace Renderer
{
    class Window
    {

    public:
        Window();
        Window(uint32_t width,uint32_t height,const char* title);
        Window(const Window& other) = delete;
        ~Window() = default;

        bool Render();
        GLFWwindow* GetWindow() const {return window;}

    public:
    
    public:
        std::unique_ptr<EventsHandler::Keyboard> kbr;
    
    private:
        GLFWwindow* window;
        uint32_t width;
        uint32_t height;
        const char* title;


    };
}
