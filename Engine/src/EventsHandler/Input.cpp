
#include "Input.hpp"

static void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    auto mouse = reinterpret_cast<EventsHandler::Input *>(glfwGetWindowUserPointer(window));
    mouse->mouse->Process(xpos, ypos);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    auto kbd = reinterpret_cast<EventsHandler::Input *>(glfwGetWindowUserPointer(window));
    kbd->kbr->Set(key, action);
}

EventsHandler::Input::Input(GLFWwindow *window)
{
    kbr = std::make_unique<EventsHandler::Keyboard>();
    mouse = std::make_unique<EventsHandler::Mouse>();

    glfwSetWindowUserPointer(window, this);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
}
