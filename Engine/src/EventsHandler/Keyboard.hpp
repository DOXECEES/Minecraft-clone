
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <bitset>
#include <queue>
#include <string>
#include <functional>

#include "../Logger.hpp"

namespace EventsHandler
{
    class Keyboard
    {

    static constexpr uint16_t MAX_KEY_CODE = 348;  // 348 is the biggest number in GLFW key codes (GLFW_KEY_MENU)

    public:

        Keyboard(GLFWwindow *Window);
        Keyboard(const Keyboard& other) = default;
        Keyboard& operator=(const Keyboard&) = default;

        ~Keyboard() = default;

        bool isPressed(uint16_t keyCode);
        bool isReleased(uint16_t keyCode);

        void Set(uint16_t keyCode, bool state);


    private:

        std::bitset<MAX_KEY_CODE> keys; 
    };


};
