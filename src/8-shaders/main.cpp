#define GLFW_INCLUDE_NONE
#include <stdlib.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <utils.h>
#include <exception>

int main()
{
	try 
	{
		Glfw glfw;
		unsigned windowWidth = 800, windowHeight = 800;
		Window window(windowWidth, windowHeight);
		window.makeCurrent();

		loadGlad();

		float squarePositions[8] = {
			-0.5f, -0.5f,
			0.5f, -0.5f,
			0.5f, 0.5f,
			-0.5f, 0.5f,
		};

		unsigned int squareIndices[6] = {
			0, 1, 2,
			2, 3, 0
		};
		
		std::string shaderFile("/home/ana/sim-experiments/src/8-shaders/shader.txt");
		std::string shaderSource = readFromFile(shaderFile);
		std::cout << "arquivo shader lido" << std::endl;

		unsigned int vao;
		glGenBuffers(1, &vao);

		unsigned int vbo;
		glGenBuffers(1, &vbo);

		unsigned int ebo;
		glGenBuffers(1, &ebo);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(squarePositions), squarePositions, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		while (!window.shouldClose())
		{
			glClear(GL_COLOR_BUFFER_BIT);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, squareIndices);

			window.swapBuffers();

			glfwPollEvents();
		}
	}

	catch(std::exception& e)
	{
		std::cout << "exception caught: " << e.what() << std::endl;
	}

	return 0;
}
