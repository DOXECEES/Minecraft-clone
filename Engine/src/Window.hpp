
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace Render
{
    class Window
    {

    public:
        Window();
        Window(uint32_t width,uint32_t height,const char* title);
        Window(const Window& other) = delete;

        ~Window() = default;

        bool Render();
        


    private:
        GLFWwindow* window;
        uint32_t width;
        uint32_t height;
        const char* title;


    };
}
