
#include "Keyboard.hpp"

EventsHandler::Keyboard::Keyboard()
{
}

void EventsHandler::Keyboard::Set(uint16_t keyCode, bool state)
{
    if (keyCode < MAX_KEY_CODE)
    {
        if (state == true)
        {
            keys[keyCode] = true;
            Logger::Log("Keyboard code " + std::to_string(keyCode) + " was pressed", Logger::INFO);
        }
        else
        {
            keys[keyCode] = false;
            Logger::Log("Keyboard code " + std::to_string(keyCode) + " was released", Logger::INFO);
        }
    }
    else
    {
        Logger::Log("Keyboard code " + std::to_string(keyCode) + " not allowed", Logger::ERROR);
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
