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
#include <mesh.h>

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

        // centro do objeto (sem considerar um vetor "extendido")
        glm::vec2 objCenter(vertices[0], vertices[1]);

		std::vector<unsigned int> indices{
				0, 1
		};

		float dq = 0.01f;

		glm::mat3 aMat(0.0f, 0.0f, 0.0f, 
			       0.0f, 0.0f, 0.0f,
			       0.5f, 0.0f, 0.0f);

		std::string vertexShaderPath("/home/ana/sim-experiments/src/8-shaders/default.vert");
		std::string fragmentShaderPath("/home/ana/sim-experiments/src/8-shaders/default.frag");

        
		Object obj(vertices, indices, objCenter, vertexShaderPath, fragmentShaderPath);
        obj.setPrimitive(Primitive::line);
		obj.setAccelerationMatrix(aMat, dq);
        
        float dTime = obj.getExternTimeInterval();
        float dProperTime = obj.getProperTimeInterval();

		float time = 0.0f;
		float properTime = 0.0f;

		while (!window.shouldClose())
		{
			glClear(GL_COLOR_BUFFER_BIT);

			glUniform4f(obj.getShaderColorUniform(), 1.0f, 1.0f, 1.0f, 1.0f);
			glUniform1f(obj.getShaderTimeUniform(), time);
			glUniform1f(obj.getShaderProperTimeUniform(), properTime);
			time += dTime;
			properTime += dProperTime;

			obj.draw();

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
