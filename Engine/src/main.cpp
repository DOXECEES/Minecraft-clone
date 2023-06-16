#define GLFW_INCLUDE_NONE

#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include <GL/GL.h>

#include <glm.hpp>
#include <iostream>
//#include <GL/glew.h>

#include "Render/Window.hpp"
#include "Render/VertexArray.hpp"
#include "Render/VertexBuffer.hpp"
#include "Render/Shader.hpp"
#include "Logger.hpp"

void error_callback(int error, const char* description)
{
	std::cout << "[ERROR CODE]: " << error << std::endl <<
				 "[DESCRIPTION] " << description << std::endl;
} 

std::vector<GLfloat> verticies = 
	{-0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f};




int main()
{
	
	
	Logger::EnableConsoleLogging();
	Logger::StartLogging();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	Logger::Log("Starting..." ,Logger::INFO);

	

	//glfwSetErrorCallback(error_callback);

	

	Renderer::Window *wnd = new Renderer::Window(1920,1080,"ass");

	Renderer::Shader s = Renderer::Shader();
	s.LoadShader("Shaders\\ver.vs");
	s.LoadShader("Shaders\\frag.fs");

	s.CreateProgram();

	Renderer::VertexArray aa;
	aa.Link(verticies);

	//
	while (wnd->Render())
	{
        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw our first triangle
		glUseProgram(s.GetProgram());
        aa.Draw(3);
        

	}

	glfwTerminate();
}