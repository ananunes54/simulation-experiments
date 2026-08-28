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
#include <window.h>
#include <transform.h>
#include <camera.h>
#include <gui.h>

#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

void initialize(Window& window, Camera& camera, Transform& model);


int main()
{
	try 
	{
		int windowWidth = 800, windowHeight = 800;
		Window window(windowWidth, windowHeight, "window");
 
		std::string vertexShaderPath("/home/ana/sim-experiments/src/8-shaders/proper-diagram.vert");
		std::string fragmentShaderPath("/home/ana/sim-experiments/src/8-shaders/default.frag");

        glm::mat4 modelMat(1.0f);

        Transform modelTransform;
        Camera cam;

        initialize(window, cam, modelTransform);

        glm::vec3 linearAccel(0.0f);
        glm::vec3 angularAccel(0.0f);
        float properAngVelocity = 0.0f;

        glm::vec3 objectPosition(0.0f);
        glm::vec3 objectRotation(0.0f);
        float objectScale(1.0f);

        glm::vec3 viewPosition(0.0f);
        glm::vec3 viewTarget(0.0f);
        glm::vec3 viewUp(0.0f, 1.0f, 0.0f);

        SimulationState state;
        bool matrixAltered = false;
        bool objectAltered = true;
        bool viewAltered = true;

        glm::mat4 mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
        // centro do objeto (sem considerar um vetor "extendido")
        glm::vec3 objCenter(0.0f);


        Physics physics(state.dTime);
        Geometry geometry;
        Mesh mesh;
        Shader shader(vertexShaderPath, fragmentShaderPath);
        Render render;

        parseObj("3dwheel.obj", geometry);
        mesh.createMesh(geometry);

        physics.setProperAngVelocity(properAngVelocity);
        physics.setCenter(objCenter, mat);

        physics.log("/home/ana/sim-experiments/physics-log.txt");

        glm::mat4 modelMatRotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        shader.setGlmMat4("u_modelMatRotation", modelMatRotation);

		while (!window.shouldClose())
		{

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            render.render(mesh, physics, shader, state, cam, modelTransform);

            window.initImGuiFrame();

            ///////////////
            ImGui::Begin("Controles");

            if (UIWidget::drawDualButton("Continuar", "Pausar", state.paused))
            {
                state.paused = !state.paused;
            }

            ImGui::SameLine();

            if (UIWidget::drawButton("Reset"))
            {
                linearAccel = glm::vec3(0.0f);
                angularAccel = glm::vec3(0.0f);
                properAngVelocity = 0.0f;
                physics.setProperAngVelocity(properAngVelocity);
                matrixAltered = true;
                state.paused = true;
            }

            if (UIWidget::drawVec3Control("Aceleração linear", linearAccel, glm::vec2(-100.0f, 100.0f), "%.2f", glm::vec3(0.0f)))
            {
                matrixAltered = true;
            }

            if (UIWidget::drawVec3Control("Aceleração angular", angularAccel, glm::vec2(-50.0f, 50.0f), "%.2f", glm::vec3(0.0f)))
            {
                matrixAltered = true;
            }

            if (UIWidget::drawFloatControl("Rotação (referencial próprio)", properAngVelocity, glm::vec2(-1.0f, 1.0f), "%.2f", 0.0f))
            {
                state.time = 0.0f;
                physics.setProperTime(0.0f);
                physics.setProperAngVelocity(properAngVelocity);
                state.paused = true;
                state.stateAltered = true;
            }

            ImGui::End();
            ///////////////


            ///////////////
            ImGui::Begin("Posição do objeto");

            if (UIWidget::drawVec3Control("Translação", objectPosition, glm::vec2(-50.0f, 50.0f), "%.0f", glm::vec3(0.0f)))
            {
                objectAltered = true;
            }

            if (UIWidget::drawVec3Control("Rotação", objectRotation, glm::vec2(-180.0f, 180.0f), "%.0f", glm::vec3(0.0f)))
            {
                objectAltered = true;
            }

            if (UIWidget::drawFloatControl("Escala", objectScale, glm::vec2(0.0f, 5.0f), "%.2f", 1.0f))
            {
                objectAltered = true;
            }

            if (UIWidget::drawVec3Control("Posição camera", viewPosition, glm::vec2(-50.0f, 50.0f), "%.0f", glm::vec3(0.0f)))
            {
                viewAltered = true;
            }

            if (UIWidget::drawVec3Control("Alvo camera", viewTarget, glm::vec2(-50.0f, 50.0f), "%.0f", glm::vec3(0.0f)))
            {
                viewAltered = true;
            }

            
            ImGui::End();
            ///////////////

            
            window.renderImGui();

			window.swapBuffers();

            window.pollEvents();


            ///////////////
            if (objectAltered)
            {
                modelTransform.set(objectPosition, objectRotation, objectScale);
                objectAltered = false;
            }

            if (viewAltered)
            {
                cam.setViewMat(viewPosition, viewTarget, viewUp);
                viewAltered = false;
            }


            if (matrixAltered)
            {
                state.time = 0.0f;
                state.stateAltered = true;
                physics.reset();
                physics.buildPoincareGenerator(linearAccel, angularAccel, glm::vec4(0.0f));
                physics.setCenter(objCenter, mat);
                matrixAltered = false;
                state.paused = true;
            }

            if (!state.paused)
            {
                state.time += state.dTime;
                physics.update();
            }


		}
	}

	catch(std::exception& e)
	{
		std::cout << "exception caught: " << e.what() << std::endl;
	}

	return 0;
}



void initialize(Window& window, Camera& camera, Transform& model)
{
    model.set(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(90.0f, 0.0f, 0.0f), 0.25f);
    camera.setProjectionMat(45.0f, window.getAspectRatio(), 0.1f, 100.0f);
}
