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

void error_callback(int error, const char* description)
{
	std::cout << "[ERROR CODE]: " << error << std::endl <<
				 "[DESCRIPTION] " << description << std::endl;
} 

std::vector<GLfloat> verticies = 
	{-0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f};


const GLchar* vertexShaderSource = "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "void main()\n"
      "{\n"
      "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const GLchar* fragmentShaderSource = R"(
      #version 330 core
      out vec4 lineColor;
      void main()
      {
          lineColor = vec4(1.0f, 0.1f, 0.2f, 1.0f);
      }
)";



int main()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//glfwSetErrorCallback(error_callback);

	

	Renderer::Window *wnd = new Renderer::Window(1920,1080,"ass");

	//
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);



	GLuint shaderProgram;
	shaderProgram = glCreateProgram();


	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

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
		glUseProgram(shaderProgram);
        aa.Draw(3);
        // Swap the screen buffers
        

	}

	glfwTerminate();
}