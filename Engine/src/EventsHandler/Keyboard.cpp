
#include "Keyboard.hpp"
static void key_callback (GLFWwindow * window, int key, int scancode, int action, int mods) 
{
    EventsHandler::Keyboard *kbd = reinterpret_cast<EventsHandler::Keyboard *>(glfwGetWindowUserPointer(window)); 
    kbd->Set(key,action);
}


EventsHandler::Keyboard::Keyboard(GLFWwindow *Window)
{
    glfwSetWindowUserPointer(Window, reinterpret_cast<void *>(this));
    glfwSetKeyCallback(Window, key_callback);
}


void EventsHandler::Keyboard::Set(uint16_t keyCode, bool state)
{
    if(keyCode < MAX_KEY_CODE)
    {
        if(state == true)
        {
            keys[keyCode] = true;
            Logger::Log("Keyboard code " + std::to_string(keyCode) + "was pressed",Logger::INFO);
        }
        else 
        {
            keys[keyCode] = false;
            Logger::Log("Keyboard code " + std::to_string(keyCode) + "was released",Logger::INFO);
        }
    }
    else 
    {
        Logger::Log("Keyboard code " + std::to_string(keyCode) + "not allowed",Logger::ERROR);
    }
}


bool EventsHandler::Keyboard::isPressed(uint16_t keyCode)
{
    return keys[keyCode] ? true : false;
}


bool EventsHandler::Keyboard::isReleased(uint16_t keyCode)
{
    return keys[keyCode] ? false : true;

}
