#define GLFW_INCLUDE_NONE
#include <stdlib.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
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


	float positions[6] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0, 0.5f 
	};
	

	unsigned int VAO;
	glGenBuffers(1, &VAO);

	unsigned int buffer;
	glGenBuffers(1, &buffer);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2*sizeof(float), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}


	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
