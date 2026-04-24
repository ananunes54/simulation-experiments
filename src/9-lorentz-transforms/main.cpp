#define GLFW_INCLUDE_NONE
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <matrix.h>
#include <math.h>
#include <control.h>
#include <iomanip>
#include <utils.h>

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 900


int main()
{

	GLFWwindow* window;
	int initialized = init(window, WINDOW_WIDTH, WINDOW_HEIGHT);

	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}

