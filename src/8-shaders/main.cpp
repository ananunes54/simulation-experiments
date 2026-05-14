#define GLFW_INCLUDE_NONE
#include <stdlib.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <utils.h>
#include <exception>
#include <shaders.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <matrix.h>
#include <object.h>

int main()
{
	try 
	{
		Glfw glfw;
		unsigned windowWidth = 800, windowHeight = 800;
		Window window(windowWidth, windowHeight);
		window.makeCurrent();

		loadGlad();

		std::vector<float> vertices{
			-0.5f / 2.0,  -0.5f / 2.0f,
			 0.5f / 2.0f, -0.5f / 2.0f,
			 0.5f / 2.0f,  0.5f / 2.0f,
			-0.5f / 2.0f,  0.5f / 2.0f

		};

		std::vector<unsigned int> indices{
				0, 1, 2,
				2, 3, 0
		};


		matrix<3, 3> aMat = {0.0f, 0.0f, 0.005f, 
				  0.0f, 0.0f, 0.0f,
				  0.0f, 0.0f, 0.0f};

		matrix<3, 3> omegaMat = Exponential(aMat);

		glm::mat3 lorentzMat = glm::mat3(omegaMat.data[0], omegaMat.data[3], omegaMat.data[6],
					omegaMat.data[1], omegaMat.data[4], omegaMat.data[7],
					omegaMat.data[2], omegaMat.data[5], omegaMat.data[8]);

		glm::mat3 auxMat = glm::mat3(omegaMat.data[0], omegaMat.data[3], omegaMat.data[6],
					omegaMat.data[1], omegaMat.data[4], omegaMat.data[7],
					omegaMat.data[2], omegaMat.data[5], omegaMat.data[8]);


		Object obj(vertices, indices);
		obj.setAttribute(0, 2, 2*sizeof(float), 0);	


		std::string vertexShaderPath("/home/ana/sim-experiments/src/8-shaders/default.vert");
		std::string vertexShaderStr = readFromFile(vertexShaderPath);
		std::string fragmentShaderPath("/home/ana/sim-experiments/src/8-shaders/default.frag");
		std::string fragmentShaderStr = readFromFile(fragmentShaderPath);
		unsigned int program = createProgram(vertexShaderStr, fragmentShaderStr);

		int u_colorLocation = glGetUniformLocation(program, "u_color");
		int u_timeLocation = glGetUniformLocation(program, "u_time");
		int u_lorentzMatLocation = glGetUniformLocation(program, "u_lorentzMat");

		if (program != -1)
		{
			std::cout << "opengl program compilado com sucesso" << std::endl;
			glUseProgram(program);
		}

		else
		{
			std::cout << "nao foi possivel compilar opengl program" << std::endl;
		}
		
		float time = 0.0f;
		while (!window.shouldClose())
		{
			glClear(GL_COLOR_BUFFER_BIT);

			glUniform4f(u_colorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
			glUniform1f(u_timeLocation, time);
			time += 0.01f;
			glUniformMatrix3fv(u_lorentzMatLocation, 1, GL_FALSE, glm::value_ptr(lorentzMat));

			lorentzMat = lorentzMat * auxMat;

			obj.draw();

			window.swapBuffers();

			glfwPollEvents();
		}
		
		glDeleteProgram(program);
	}



	catch(std::exception& e)
	{
		std::cout << "exception caught: " << e.what() << std::endl;
	}

	return 0;
}
