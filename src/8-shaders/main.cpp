#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <exception>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include <utils.h>
#include <geometry.h>
#include <mesh.h>
#include <physics.h>
#include <shaders.h>
#include <render.h>

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

        Geometry geometry(vertices, indices, objCenter);
        Mesh mesh(geometry);
        mesh.setPrimitive(Primitive::line);

		float dq = 0.01f;
		glm::mat3 aMat(0.0f, 0.0f, 0.0f, 
			       0.0f, 0.0f, 0.0f,
			       0.5f, 0.0f, 0.0f);

        Physics physics;
        physics.setCenter(objCenter);
        physics.setAccelerationMat(aMat, dq);

		std::string vertexShaderPath("/home/ana/sim-experiments/src/8-shaders/default.vert");
		std::string fragmentShaderPath("/home/ana/sim-experiments/src/8-shaders/default.frag");

        Shader shader(vertexShaderPath, fragmentShaderPath);

        float dTime = physics.getExternTimeInterval();
        float dProperTime = physics.getProperTimeInterval();

		float time = 0.0f;
		float properTime = 0.0f;

		while (!window.shouldClose())
		{
			glClear(GL_COLOR_BUFFER_BIT);

			glUniform4f(shader.getColorUniform(), 1.0f, 1.0f, 1.0f, 1.0f);
			glUniform1f(shader.getTimeUniform(), time);
			glUniform1f(shader.getProperTimeUniform(), properTime);
			time += dTime;
			properTime += dProperTime;

            render(mesh, physics, shader);

            physics.updateMotionMat();

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
