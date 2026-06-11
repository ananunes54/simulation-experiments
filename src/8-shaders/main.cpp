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

        glm::vec3 fourPosition = glm::vec3(vertices[0], vertices[1], 1.0f);

		std::vector<unsigned int> indices{
				0, 1
		};

		float dq = 0.01f;

		glm::mat3 aMat(0.0f, 0.0f, 0.0f, 
			       0.0f, 0.0f, 0.0f,
			       0.5f, 0.0f, 0.0f);

		std::string vertexShaderPath("/home/ana/sim-experiments/src/8-shaders/default.vert");
		std::string fragmentShaderPath("/home/ana/sim-experiments/src/8-shaders/default.frag");
	
		unsigned int program = createProgram(vertexShaderPath, fragmentShaderPath);

		int u_colorLocation = glGetUniformLocation(program, "u_color");
		int u_timeLocation = glGetUniformLocation(program, "u_time");
		int u_properTimeLocation = glGetUniformLocation(program, "u_properTime");

		Object obj(vertices, indices);
        obj.setPrimitive(Primitive::line);
		obj.setAttribute(0, 2, 2*sizeof(float), 0);
        obj.setFourPosition(fourPosition);
		obj.setAccelerationMatrix(aMat, dq);
		obj.setProgram(program);
        
        float dTime = obj.getExternTimeInterval();
        float dProperTime = obj.getProperTimeInterval();

		float time = 0.0f;
		float properTime = 0.0f;

		while (!window.shouldClose())
		{
			glClear(GL_COLOR_BUFFER_BIT);

			glUniform4f(u_colorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
			glUniform1f(u_timeLocation, time);
			glUniform1f(u_properTimeLocation, properTime);
			time += dTime;
			properTime += dProperTime;

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
