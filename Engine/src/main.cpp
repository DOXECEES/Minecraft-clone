#define GLFW_INCLUDE_NONE

#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include <GL/GL.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
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


std::vector<GLfloat> cube_vertices = {
    // front
    -1.0, -1.0,  1.0,
     1.0, -1.0,  1.0,
     1.0,  1.0,  1.0,
    -1.0,  1.0,  1.0,
    // back
    -1.0, -1.0, -1.0,
     1.0, -1.0, -1.0,
     1.0,  1.0, -1.0,
    -1.0,  1.0, -1.0
  };

std::vector<GLuint> cube_elements = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

std::vector<glm::vec3> pos = {
    {0.0f,2.0f,-5.0f},
    {2.0f,2.0,-5.0f},
    {4.0f,2.0,-5.0f},
    {7.0f,2.0,-5.0f}

};

glm::vec3 camPos(0.0f, 2.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

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



    glViewport(0, 0, 1920, 1080);

	Renderer::Shader s = Renderer::Shader();
	s.LoadShader("res\\Shaders\\ver.vs");
	s.LoadShader("res\\Shaders\\frag.fs");

	s.CreateProgram();

	Renderer::VertexArray aa;
	aa.Link(cube_vertices,cube_elements);

    //glEnable(GL_DEPTH_TEST);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

	
	while (wnd->Render())
	{
        //d
        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        int a = 0;
		s.Use();

        // Draw our first triangle
        GLfloat time = glfwGetTime();
        GLfloat color = (sin(time) / 2) + 0.5;


        glUniform4f(glGetUniformLocation(s.GetProgram(),"time"),0.20f,0.85f,0.57f,1.0f);
        glm::mat4 view;
        GLfloat radius = 10.0f;
        //GLfloat camX = sin(glfwGetTime()) * radius;
        //GLfloat camZ = cos(glfwGetTime()) * radius;

        // View
        view = glm::lookAt(camPos, camPos + cameraFront, glm::vec3(0.0, 1.0, 0.0));
        // Projection 
        glm::mat4 projection;
        projection = glm::perspective(90.0f, (GLfloat)1920 / (GLfloat)1080, 0.1f, 100.0f);
        // Model 
       
        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(s.GetProgram(),"model");
        GLint viewLoc = glGetUniformLocation(s.GetProgram(), "view");
        GLint projLoc = glGetUniformLocation(s.GetProgram(), "projection");
        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Camera control
        if(wnd->kbr->isPressed(GLFW_KEY_A))
        {
            camPos = camPos + glm::vec3(-0.01f,0.0f,0.0f);
        } 
        if(wnd->kbr->isPressed(GLFW_KEY_D))
        {
            camPos = camPos + glm::vec3(0.01f,0.0f,0.0f);
        } 
        if(wnd->kbr->isPressed(GLFW_KEY_S))
        {
            camPos = camPos + glm::vec3(0.0f,0.0f,0.01f);
        } 
        if(wnd->kbr->isPressed(GLFW_KEY_W))
        {
            camPos = camPos + glm::vec3(0.0f,0.0f,-0.01f);
        } 
        //

        for(int i = 0;i<pos.size();i++)
        {
            glm::mat4 model;
            model = glm::translate(glm::mat4(1.0f), pos[i]);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            aa.Draw(36);
        }

        

	}

	glfwTerminate();
}