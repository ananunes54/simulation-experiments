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
#include <material.h>


int main()
{
	try 
	{
		Glfw glfw;
		unsigned windowWidth = 800, windowHeight = 800;
		Window window(windowWidth, windowHeight);
		window.makeCurrent();
        loadGlad();

		std::vector<glm::vec3> vertices{
             glm::vec3(0.0f / 1.0f,  0.5f / 2.0f,  0.5f / 2.0f), 
			 glm::vec3(0.0f / 1.0f, -0.5f / 2.0f,  0.5f / 2.0f),
			 glm::vec3(0.0f / 1.0f, -0.5f / 2.0f, -0.5f / 2.0f),
			 glm::vec3(0.0f / 1.0f,  0.5f / 2.0f, -0.5f / 2.0f),
		};

        // centro do objeto (sem considerar um vetor "extendido")
        glm::vec3 objCenter(0.0f, 0.0f, 0.0f);

		std::vector<unsigned int> indices{
				0, 1,
                1, 2,
                2, 3,
                3, 0
		};

        Geometry geometry(vertices, indices, objCenter);
        Mesh mesh(geometry);
        mesh.setPrimitive(Primitive::line);

		float dq = 0.01f;
		glm::mat4 aMat(0.0f, 0.0f, 0.0f, 0.0f,
			       0.0f, 0.0f, 0.0f, 0.0f,
			       0.0f, 0.0f, 0.0f, 0.0f,
			       0.5f, 0.3f, 0.0f, 0.0f);

        Physics physics;
        physics.setCenter(objCenter);
        physics.setAccelerationMat(aMat, dq);

		std::string vertexShaderPath("/home/ana/sim-experiments/src/8-shaders/default.vert");
		std::string fragmentShaderPath("/home/ana/sim-experiments/src/8-shaders/default.frag");

        Shader shader(vertexShaderPath, fragmentShaderPath);
        Material material(shader);

        std::cout << "gamma: " << physics.getGamma() << std::endl;
        std::cout << "velocity: " << physics.getVelocityMagnitude() << std::endl;

        float dTime = physics.getExternTimeInterval();
        float dProperTime = physics.getProperTimeInterval();

        std::cout << "dT: " << dProperTime << std::endl;

		float time = 0.0f;
		float properTime = 0.0f;

		while (!window.shouldClose())
		{
			glClear(GL_COLOR_BUFFER_BIT);

            std::string uColor = "u_color";
            material.setGlmVec4(uColor, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            
            std::string uTime = "u_time";
            material.setFloat(uTime, time);
			
            std::string uProperTime = "u_properTime";
            material.setFloat(uProperTime, properTime);
			
			time += dTime;
			properTime += dProperTime;

            render(mesh, physics, material);

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
