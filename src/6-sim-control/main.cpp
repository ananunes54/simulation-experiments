#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <exception>
#include <utils.h>
#define WAYLAND


int main ()
{
	try
	{
		Glfw glfw;
		Window window1(720, 720);
		
		window1.makeCurrent();
		loadGlad();

		while (!window1.shouldClose())
		{
			glClear(GL_COLOR_BUFFER_BIT);

			window1.swapBuffers();

			glfwPollEvents();
		}
	}

	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}
