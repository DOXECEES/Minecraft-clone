#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
int main()
{
	if (!glfwInit()) {
		std::cout << "GLFW initialization failed!" << std::endl;
		exit(EXIT_FAILURE);
	}
	GLFWwindow* window;
	window = glfwCreateWindow(640, 480, "dsa", nullptr, nullptr);
	while (true)
	{

	}
	glfwTerminate();
}