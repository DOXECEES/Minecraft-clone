
#include "Window.hpp"

Render::Window::Window()
    :width(640),height(480),title("Engine")
{
    window = glfwCreateWindow(width,height,title,nullptr,nullptr);

    if(!window)
	{
		std::cout << "Failed to create window" << std::endl;
		exit(EXIT_FAILURE);
	}

    glfwMakeContextCurrent(window);

}


Render::Window::Window(uint32_t width,uint32_t height,const char* title)
    :width(width),height(height),title(title)
{
     window = glfwCreateWindow(width,height,title,nullptr,nullptr);

    if(!window)
	{
		std::cout << "Failed to create window" << std::endl;
		exit(EXIT_FAILURE);
	}

    glfwMakeContextCurrent(window);

}


bool Render::Window::Render()
{   
    if(!glfwWindowShouldClose(window))
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
