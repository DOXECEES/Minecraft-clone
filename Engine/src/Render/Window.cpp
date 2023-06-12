
#include "Window.hpp"

Renderer::Window::Window()
    :width(640),height(480),title("Engine")
{
    if (!glfwInit()) 
	{
		std::cout << "GLFW initialization failed!" << std::endl;
		exit(EXIT_FAILURE);
	}

    window = glfwCreateWindow(width,height,title,nullptr,nullptr);

    if(!window)
	{
		std::cout << "Failed to create window" << std::endl;
		exit(EXIT_FAILURE);
	}

    glfwMakeContextCurrent(window);

     if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "GLAD initialization failed " << std::endl;
		exit(EXIT_FAILURE);
    }
}


Renderer::Window::Window(uint32_t width,uint32_t height,const char* title)
    :width(width),height(height),title(title)
{
    if (!glfwInit()) 
	{
		std::cout << "GLFW initialization failed!" << std::endl;
		exit(EXIT_FAILURE);
	}

     window = glfwCreateWindow(width,height,title,nullptr,nullptr);

    if(!window)
	{
		std::cout << "Failed to create window" << std::endl;
		exit(EXIT_FAILURE);
	}

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "GLAD initialization failed " << std::endl;
		exit(EXIT_FAILURE);
    }

}


bool Renderer::Window::Render()
{   
    glfwMakeContextCurrent(window);

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

