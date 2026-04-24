#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <utils.h>
#include <iostream>


int init(GLFWwindow*& window, unsigned int windowW, unsigned int windowH)
{
	glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
		
	if (!glfwInit())
	{
		return -1;
	}

	window = glfwCreateWindow(windowW, windowH, "window", NULL, NULL);

	if (!window)
	{
		std::cout << "erro ao criar janela." << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "erro ao carregar glad." << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	return 0;
}


