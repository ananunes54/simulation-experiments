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
             glm::vec3(0.0f, -0.3f,  0.1f / 2.0f), 
             glm::vec3(0.0f, -0.2f,  0.1f / 2.0f), 
             glm::vec3(0.0f, -0.1f,  0.1f / 2.0f), 
             glm::vec3(0.0f,  0.0f,  0.1f / 2.0f), 
             glm::vec3(0.0f,  0.1f,  0.1f / 2.0f), 
             glm::vec3(0.0f,  0.2f,  0.1f / 2.0f), 
             glm::vec3(0.0f,  0.3f,  0.1f / 2.0f), 
             glm::vec3(0.0f,  0.3f, -0.1f / 2.0f), 
             glm::vec3(0.0f,  0.2f, -0.1f / 2.0f), 
             glm::vec3(0.0f,  0.1f, -0.1f / 2.0f), 
             glm::vec3(0.0f, -0.1f, -0.1f / 2.0f), 
             glm::vec3(0.0f, -0.2f, -0.1f / 2.0f), 
             glm::vec3(0.0f, -0.3f, -0.1f / 2.0f), 

             glm::vec3(0.0f,  0.1f / 2.0f, -0.3f), 
             glm::vec3(0.0f,  0.1f / 2.0f, -0.2f), 
             glm::vec3(0.0f,  0.1f / 2.0f, -0.1f), 
             glm::vec3(0.0f,  0.1f / 2.0f,  0.0f), 
             glm::vec3(0.0f,  0.1f / 2.0f,  0.1f), 
             glm::vec3(0.0f,  0.1f / 2.0f,  0.2f), 
             glm::vec3(0.0f,  0.1f / 2.0f,  0.3f), 
             glm::vec3(0.0f, -0.1f / 2.0f,  0.3f), 
             glm::vec3(0.0f, -0.1f / 2.0f,  0.2f), 
             glm::vec3(0.0f, -0.1f / 2.0f,  0.1f), 
             glm::vec3(0.0f, -0.1f / 2.0f, -0.1f), 
             glm::vec3(0.0f, -0.1f / 2.0f, -0.2f), 
             glm::vec3(0.0f, -0.1f / 2.0f, -0.3f), 
		};
 
        glm::mat4 modelMat(1.0f);
        modelMat = glm::translate(modelMat, glm::vec3(0.0f, -0.05f, 0.0f));
        // centro do objeto (sem considerar um vetor "extendido")
        glm::vec3 objCenter(0.0f, 0.0f, 0.0f);

		std::vector<unsigned int> indices{
				0, 1,
                1, 2,
                2, 3,
                3, 4,
                4, 5,
                5, 6,
                6, 7,
                7, 8,
                8, 9,
                9, 10,
                10, 11,
                11, 12,
                12, 0,

                13, 14,
                14, 15,
                15, 16,
                16, 17,
                17, 18,
                18, 19,
                19, 20,
                20, 21,
                21, 22,
                22, 23,
                23, 24,
                24, 25,
                25, 13
		};

        Geometry geometry(vertices, indices, objCenter);
        Geometry geometry2("3dwheel.obj");
        Mesh mesh(geometry);
        mesh.createMeshFromObj(geometry2);

        mesh.setPrimitive(Primitive::line);

		float dq = 0.01f;
		glm::mat4 generatorMat(0.0f, 70.0f, 0.0f, 0.0f,
			       70.0f, 0.0f, 0.0f, 0.0f,
			       0.0f, 0.0f, 0.0f, 0.0f,
			       0.0f, 0.0f, 0.0f, 0.0f);

        Physics physics;
        physics.setCenter(objCenter, modelMat);
        physics.setGroupGeneratorMat(generatorMat, dq, MOTION::inertial);

		std::string vertexShaderPath("/home/ana/sim-experiments/src/8-shaders/proper-diagram.vert");
		std::string fragmentShaderPath("/home/ana/sim-experiments/src/8-shaders/default.frag");

        Shader shader(vertexShaderPath, fragmentShaderPath);
        Material material(shader);

        float dTime = physics.getExternTimeInterval();
        float dProperTime = physics.getProperTimeInterval();

		float time = 0.0f;
		float properTime = 0.0f;

        physics.log("/home/ana/sim-experiments/physics-log.txt");

		while (!window.shouldClose())
		{
			glClear(GL_COLOR_BUFFER_BIT);

            material.setGlmVec4("u_color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            material.setFloat("u_time", time);
            material.setFloat("u_properTime", properTime);
            material.setGlmMat4("u_poincareGroupMat", physics.getPoincareGroupMat());
            material.setFloat("u_gamma", physics.getGamma());
            material.setFloat("u_velocity", physics.getVelocityMagnitude());
            material.setGlmMat4("u_refChangeMat", physics.getRefChangeMat()); 
			
			time += dTime;
			properTime += dProperTime;

            render(mesh, physics, material);

            //physics.updatePoincareGroupMat();

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
