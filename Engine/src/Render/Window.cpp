// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "Window.hpp"

Renderer::Window::Window()
    : width(640), height(480), title("Engine")
{
    if (!glfwInit())
    {
        Logger::Log("GLFW initialization failed!", Logger::FATAL);
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (!window)
    {
        Logger::Log("Failed to create window", Logger::FATAL);
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    // kbr = std::make_unique<EventsHandler::Keyboard>(window);
    //  mouse = std::make_unique<EventsHandler::Mouse>(window);
    input = std::make_unique<EventsHandler::Input>(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Logger::Log("GLAD initialization failed ", Logger::FATAL);
        exit(EXIT_FAILURE);
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Renderer::Window::Window(uint32_t width, uint32_t height, const char *title)
    : width(width), height(height), title(title)
{
    if (!glfwInit())
    {
        Logger::Log("GLFW initialization failed!", Logger::FATAL);
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (!window)
    {
        Logger::Log("Failed to create window", Logger::FATAL);
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    input = std::make_unique<EventsHandler::Input>(window);

    // kbr = std::make_unique<EventsHandler::Keyboard>(window);
    //  mouse = std::make_unique<EventsHandler::Mouse>(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Logger::Log("GLAD initialization failed ", Logger::FATAL);
        exit(EXIT_FAILURE);
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

bool Renderer::Window::Render()
{
    glfwMakeContextCurrent(window);

    if (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
        return true;
    }
    else
    {
        return false;
    }
}
