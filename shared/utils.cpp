#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <utils.h>
#include <iostream>
#include <exception>


Glfw::Glfw()
{
	#ifdef WAYLAND
	glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
	#endif

	if (!glfwInit())
	{
		const char* description;
		int error = glfwGetError(&description);
		throw std::runtime_error(description);
	}
}

Glfw::~Glfw()
{
	std::cout << "glfw terminated." << std::endl;
	glfwTerminate();
}

Window::Window(unsigned int width, unsigned int height)
{
	#ifdef WAYLAND
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
	#endif

	handle = glfwCreateWindow(width, height, "window", NULL, NULL);

	if (!handle)
	{
		const char* description;
		int err = glfwGetError(&description);
		throw std::runtime_error(description);
	}
}

Window::~Window()
{
	std::cout << "window destroyed." << std::endl;
	glfwDestroyWindow(handle);
}

void Window::makeCurrent()
{
	glfwMakeContextCurrent(handle);
}

int Window::shouldClose()
{
	return glfwWindowShouldClose(handle);
}

void Window::swapBuffers()
{
	glfwSwapBuffers(handle);
}


void loadGlad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::runtime_error("não foi possivel carregar o glad.");
	}
}
