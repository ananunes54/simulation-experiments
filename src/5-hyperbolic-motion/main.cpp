#define GLFW_INCLUDE_NONE
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <matrix.h>

#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 720

int main()
{
	glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
		
	if (!glfwInit())
	{
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "window", NULL, NULL);

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



	double positions[8] = {
		0, 0.1,
		-0.1, 0,
		0.5, 0,
		0.7, 0.7
	};


	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_DOUBLE, GL_TRUE, 3*sizeof(double), 0);


	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_POINTS, 0, 4);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

}
