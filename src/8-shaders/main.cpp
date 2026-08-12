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
#include <math5d.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

int main()
{
	try 
	{
		int windowWidth = 800, windowHeight = 800;
		Window window(windowWidth, windowHeight, "window");
 
        glm::mat4 modelMat(1.0f);
        modelMat = glm::translate(modelMat, glm::vec3(0.0f, -0.05f, 0.0f));
        // centro do objeto (sem considerar um vetor "extendido")
        glm::vec3 objCenter(0.0f, 0.0f, 0.0f);

        Geometry geometry("3dwheel.obj");
        Mesh mesh;
        mesh.createMeshFromObj(geometry);
        //mesh.setPrimitive(Primitive::line);

		float dq = 0.01f;
        Mat5 generator(Vec5(0.0f, 70.0f, 0.0f, 0.0f, 0.0f),
                       Vec5(70.0f, 0.0f, 0.0f, 0.0f, 0.0f),
                       Vec5(0.0f),
                       Vec5(0.0f),
                       Vec5(0.0f));


        Physics physics;
        physics.setCenter(objCenter, modelMat);
        physics.setGroupGeneratorMat(generator, dq, MOTION::inertial);

		std::string vertexShaderPath("/home/ana/sim-experiments/src/8-shaders/proper-diagram.vert");
		std::string fragmentShaderPath("/home/ana/sim-experiments/src/8-shaders/default.frag");

        Shader shader(vertexShaderPath, fragmentShaderPath);
        Material material(shader);

        float dTime = physics.getExternTimeInterval();
        float dProperTime = physics.getProperTimeInterval();

		float time = 0.0f;
		float properTime = 0.0f;

        physics.log("/home/ana/sim-experiments/physics-log.txt");

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		while (!window.shouldClose())
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            material.setGlmVec4("u_color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            material.setFloat("u_time", time);
            material.setFloat("u_properTime", properTime);
            material.setGlmMat4("u_poincareGroupMat", physics.getPoincareGroupMat4());
            material.setGlmVec4("u_poincareTranslationVec", physics.getPoincareTranslationVec());
            material.setFloat("u_gamma", physics.getGamma());
            material.setFloat("u_velocity", physics.getVelocityMagnitude());
            material.setGlmMat4("u_refChangeMat", physics.getRefChangeMat()); 
			
            glm::mat4 modelMat(1.0f);
            modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, -5.0f));
            modelMat = glm::rotate(modelMat, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            modelMat = glm::scale(modelMat, glm::vec3(0.25f, 0.25f, 0.25f));
            material.setGlmMat4("u_modelMat", modelMat);

            glm::mat4 viewMat(1.0f);
            //viewMat = glm::rotate(viewMat, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            //viewMat = glm::translate(viewMat, glm::vec3(0.0f, 0.0f, -5.0f));
            material.setGlmMat4("u_viewMat", viewMat);

            glm::mat4 projectionMat(1.0f);
            projectionMat = glm::perspective(glm::radians(45.0f), window.getAspectRatio(), 0.1f, 100.0f);
            material.setGlmMat4("u_projectionMat", projectionMat);

			time += dTime;
			properTime += dProperTime;

            render(mesh, physics, material);

            //physics.updatePoincareGroupMat();

			window.swapBuffers();
			window.pollEvents();
		}
	}

	catch(std::exception& e)
	{
		std::cout << "exception caught: " << e.what() << std::endl;
	}

	return 0;
}
