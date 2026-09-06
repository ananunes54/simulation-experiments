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
#include <control.h>

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

        Transform modelTransform;
        Camera cam;

        initialize(window, cam, modelTransform);

        PhysicsConfig physicsC;
        TransformConfig transformC;
        CameraConfig cameraC;
        SimulationState state;

        Physics physics(state.dTime);
        Geometry geometry;
        Mesh mesh;
        Shader shader(vertexShaderPath, fragmentShaderPath);
        Render render;

        parseObj("3dwheel.obj", geometry);
        mesh.createMesh(geometry);

        physics.setProperAngVelocity(physicsC.properAngVelocity);

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
                state.paused = true;
                physicsC.reset();
                render.setFlag(Render::PHYSICS);
            }

            if (UIWidget::drawVec3Control("Aceleração linear", physicsC.linearAccel, glm::vec2(-100.0f, 100.0f), "%.2f", glm::vec3(0.0f)))
            {
                render.setFlag(Render::PHYSICS);
            }

            if (UIWidget::drawVec3Control("Aceleração angular", physicsC.angularAccel, glm::vec2(-50.0f, 50.0f), "%.2f", glm::vec3(0.0f)))
            {
                render.setFlag(Render::PHYSICS);
            }

            if (UIWidget::drawFloatControl("Rotação (referencial próprio)", physicsC.properAngVelocity, glm::vec2(-1.0f, 1.0f), "%.2f", 0.0f))
            {
                render.setFlag(Render::PHYSICS);
            }

            ImGui::End();
            ///////////////


            ///////////////
            ImGui::Begin("Posição do objeto");

            if (UIWidget::drawVec3Control("Translação", transformC.objectPosition, glm::vec2(-50.0f, 50.0f), "%.0f", glm::vec3(0.0f)))
            {
                render.setFlag(Render::TRANSFORM);
            }

            if (UIWidget::drawVec3Control("Rotação", transformC.objectRotation, glm::vec2(-180.0f, 180.0f), "%.0f", glm::vec3(0.0f)))
            {
                render.setFlag(Render::TRANSFORM);
            }

            if (UIWidget::drawFloatControl("Escala", transformC.objectScale, glm::vec2(0.0f, 5.0f), "%.2f", 1.0f))
            {
                render.setFlag(Render::TRANSFORM);
            }

            if (UIWidget::drawVec3Control("Posição camera", cameraC.viewPosition, glm::vec2(-50.0f, 50.0f), "%.0f", glm::vec3(0.0f)))
            {
                render.setFlag(Render::CAMERA);
            }

            if (UIWidget::drawVec3Control("Alvo camera", cameraC.viewTarget, glm::vec2(-50.0f, 50.0f), "%.0f", glm::vec3(0.0f)))
            {
                render.setFlag(Render::CAMERA);
            }

            
            ImGui::End();
            ///////////////

            
            window.renderImGui();

			window.swapBuffers();

            window.pollEvents();


            ///////////////
            
            if (render.checkFlag(Render::TRANSFORM))
            {
                modelTransform.set(transformC.objectPosition, transformC.objectRotation, transformC.objectScale);
            }

            if (render.checkFlag(Render::CAMERA))
            {
                cam.setViewMat(cameraC.viewPosition, cameraC.viewTarget, cameraC.viewUp);
            }


            if (render.checkFlag(Render::PHYSICS))
            {
                state.resetTime();
                physics.reset();
                physics.setProperAngVelocity(physicsC.properAngVelocity);
                physics.buildPoincareGenerator(physicsC.linearAccel, physicsC.angularAccel, glm::vec4(0.0f));
            }

            if (!state.paused)
            {
                state.updateTime();
                physics.update();
            }

            if (window.ratioChanged())
            {
                cam.setProjectionRatio(window.getAspectRatio());
                render.setFlag(Render::CAMERA);
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
