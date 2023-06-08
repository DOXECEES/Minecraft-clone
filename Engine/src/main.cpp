
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Window.hpp"

void error_callback(int error, const char* description)
{
	std::cout << "[ERROR CODE]: " << error << std::endl <<
				 "[DESCRIPTION] " << description << std::endl;
} 


int main()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);


	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) 
	{
		std::cout << "GLFW initialization failed!" << std::endl;
		exit(EXIT_FAILURE);
	}

	Render::Window *wnd = new Render::Window(1920,1080,"ass");
	
	while (wnd->Render())
	{
				

	}

	glfwTerminate();
}