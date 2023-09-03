
#include "Window.hpp"



Renderer::Window::Window()
    :width(640),height(480),title("Engine")
{
    if (!glfwInit()) 
	{
        Logger::Log("GLFW initialization failed!",Logger::FATAL);
		exit(EXIT_FAILURE);
	}

    window = glfwCreateWindow(width,height,title,nullptr,nullptr);

    if(!window)
	{
        Logger::Log("Failed to create window",Logger::FATAL);
		exit(EXIT_FAILURE);
	}    

    glfwMakeContextCurrent(window);

    kbr = std::make_unique<EventsHandler::Keyboard>(window);


     if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Logger::Log("GLAD initialization failed ",Logger::FATAL);
		exit(EXIT_FAILURE);
    }

    
}


Renderer::Window::Window(uint32_t width,uint32_t height,const char* title)
    :width(width),height(height),title(title)
{
    if (!glfwInit()) 
	{
        Logger::Log("GLFW initialization failed!",Logger::FATAL);
		exit(EXIT_FAILURE);
	}

     window = glfwCreateWindow(width,height,title,nullptr,nullptr);

    if(!window)
	{
        Logger::Log("Failed to create window",Logger::FATAL);
		exit(EXIT_FAILURE);
	}

    glfwMakeContextCurrent(window);

    kbr = std::make_unique<EventsHandler::Keyboard>(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Logger::Log("GLAD initialization failed ",Logger::FATAL);
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

