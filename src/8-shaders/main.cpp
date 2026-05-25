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
#include <object.h>

float minkowskiMetric(glm::vec3 v1, glm::vec3 v2)
{
	return v1[0] * v2[0] - v1[1] * v2[1];
}

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
			 0.0f / 1.0f, -0.5f / 2.0f,  
			 0.0f / 1.0f,  0.5f / 2.0f,
		};

		std::vector<unsigned int> indices{
				0, 1
		};


		glm::mat3 aMat(0.0f,   0.0f, 0.0f, 
			       0.0f,   0.0f, 0.0f,
			       0.005f, 0.004f, 0.0f);


		glm::mat3 motionMat = exp(aMat);

		glm::vec3 fVelocity = aMat * glm::vec3(vertices[0], vertices[1], 1);

		float velocity = fVelocity[1] / fVelocity[0];

		float gamma = 1 / sqrt(1 - pow(velocity, 2));

		
		glm::mat3 lorentzMat(
				  gamma           , - gamma * velocity, 0,
				- gamma * velocity,   gamma           , 0,
				0                 ,                  0, 1
				); 

		std::string vertexShaderPath("/home/ana/sim-experiments/src/8-shaders/default.vert");
		std::string fragmentShaderPath("/home/ana/sim-experiments/src/8-shaders/default.frag");
	
		unsigned int program = createProgram(vertexShaderPath, fragmentShaderPath);

		int u_colorLocation = glGetUniformLocation(program, "u_color");
		int u_timeLocation = glGetUniformLocation(program, "u_time");

		Object obj(Shape::line, vertices, indices);
		obj.setAttribute(0, 2, 2*sizeof(float), 0);
		obj.setMotionMatrix(motionMat);
		obj.setRefChangeMatrix(lorentzMat);
		obj.setVelocity(velocity);
		obj.setProgram(program);

		
		float time = 0.0f;
		while (!window.shouldClose())
		{
			glClear(GL_COLOR_BUFFER_BIT);

			glUniform4f(u_colorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
			glUniform1f(u_timeLocation, time);
			time += 0.01f;

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
