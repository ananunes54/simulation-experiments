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

#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

void initialize(glm::mat4& modelMat, glm::mat4& viewMat, glm::mat4& projectionMat, Window& window);
glm::mat4 setModelMat(glm::vec3& translation, glm::vec3& rotation, float scale);
glm::mat4 setViewMat(glm::vec3& position, glm::vec3& target, glm::vec3& up);

Mat5 buildPoincare(const glm::vec3& linAcceleration, const glm::vec3& angAcceleration, const glm::vec4& translation)
{
    Mat5 mat;
    mat[0] = Vec5(0.0f, linAcceleration[0], linAcceleration[1], linAcceleration[2], translation[0]);
    mat[1] = Vec5(linAcceleration[0], 0.0f, -angAcceleration[2], angAcceleration[1], translation[1]);
    mat[2] = Vec5(linAcceleration[1], angAcceleration[2], 0.0f, -angAcceleration[0], translation[2]);
    mat[3] = Vec5(linAcceleration[2], -angAcceleration[1], angAcceleration[0], 0.0f, translation[3]);
    mat[4][4] = 1.0f;

    return mat;
}

int main()
{
	try 
	{
		int windowWidth = 800, windowHeight = 800;
		Window window(windowWidth, windowHeight, "window");
 
		std::string vertexShaderPath("/home/ana/sim-experiments/src/8-shaders/proper-diagram.vert");
		std::string fragmentShaderPath("/home/ana/sim-experiments/src/8-shaders/default.frag");

        glm::mat4 modelMat(1.0f);
        glm::mat4 viewMat(1.0f);
        glm::mat4 projectionMat(1.0f);

        initialize(modelMat, viewMat, projectionMat, window);

        glm::vec3 linearAccel(0.0f, 0.0f, 0.0f);
        glm::vec3 angularAccel(0.0f, 0.0f, 0.0f);
        float properAngVelocity = 0.0f;

        glm::vec3 objectPosition(0.0f);
        glm::vec3 objectRotation(0.0f);
        float objectScale(1.0f);

        glm::vec3 viewPosition(0.0f);
        glm::vec3 viewTarget(0.0f);
        glm::vec3 viewUp(0.0f, 1.0f, 0.0f);

        bool isPaused = true;
        bool matrixAltered = false;
        bool objectAltered = true;
        bool viewAltered = true;

		float time = 0.0f;
		float properTime = 0.0f;



        glm::mat4 mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        // centro do objeto (sem considerar um vetor "extendido")
        glm::vec3 objCenter(0.0f, 0.0f, 0.0f);

        Geometry geometry("3dwheel.obj");
        Mesh mesh;
        mesh.createMeshFromObj(geometry);
        mesh.setPrimitive(Primitive::line);

		float dq = 0.01f;
        Mat5 generator(0.0f);

        Physics physics;
        physics.setProperAngVelocity(properAngVelocity);
        physics.setCenter(objCenter, mat);
        physics.setGroupGeneratorMat(generator, dq, MOTION::inertial);

        Shader shader(vertexShaderPath, fragmentShaderPath);
        Material material(shader);

        float dTime = physics.getExternTimeInterval();
        float dProperTime = physics.getProperTimeInterval();

        physics.log("/home/ana/sim-experiments/physics-log.txt");

        glm::mat4 modelMatRotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        material.setGlmMat4("u_modelMatRotation", modelMatRotation);

		while (!window.shouldClose())
		{
            window.pollEvents();

            material.setGlmVec4("u_color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            material.setFloat("u_time", time);
            material.setFloat("u_properTime", properTime);
            material.setGlmMat4("u_poincareGroupMat", physics.getPoincareGroupMat4());
            material.setGlmVec4("u_poincareTranslationVec", physics.getPoincareTranslationVec());
            material.setFloat("u_gamma", physics.getGamma());
            material.setFloat("u_velocity", physics.getVelocityMagnitude());
            material.setGlmMat4("u_refChangeMat", physics.getRefChangeMat()); 
            material.setGlmMat4("u_viewMat", viewMat);
            material.setGlmMat4("u_projectionMat", projectionMat);
            material.setFloat("u_angVelocity", physics.getProperAngVelocity());
            material.setGlmMat3("u_alignmentMat", physics.getAlignmentMat());
            material.setGlmMat3("u_alignmentMatInverse", glm::inverse(physics.getAlignmentMat()));

            window.initImGuiFrame();

            ///////////////
            ImGui::Begin("Controles");

            if (ImGui::Button(isPaused ? "Continuar" : "Pausar"))
            {
                isPaused = !isPaused;
            }

            ImGui::SameLine();

            if (ImGui::Button("Reset"))
            {
                linearAccel = glm::vec3(0.0f, 0.0f, 0.0f);
                angularAccel = glm::vec3(0.0f, 0.0f, 0.0f);
                properAngVelocity = 0.0f;
                physics.setProperAngVelocity(properAngVelocity);
                matrixAltered = true;
                isPaused = true;
            }

            if (ImGui::SliderFloat3("Aceleração Linear", glm::value_ptr(linearAccel), -100.0f, 100.0f, "%.2f"))
            {
                matrixAltered = true;
            }

            if (ImGui::SliderFloat3("Aceleração Angular", glm::value_ptr(angularAccel), -50.0f, 50.0f, "%.2f"))
            {
                matrixAltered = true;
            }

            if (ImGui::SliderFloat("Velocidade Angular (ref. próprio)", &properAngVelocity, -1.0f, 1.0f, "%.2f"))
            {
                time = properTime = 0.0f;
                physics.setProperAngVelocity(properAngVelocity);
                isPaused = true;
            }

            ImGui::End();
            ///////////////


            ///////////////
            ImGui::Begin("Posição do objeto");

            if (ImGui::Button("Reset1"))
            {
                objectAltered = true;
                objectPosition = glm::vec3(0.0f);
            }

            ImGui::SameLine();

            if (ImGui::SliderFloat3("Translação", glm::value_ptr(objectPosition), -50.0f, 50.0f, "%.0f"))
            {
                objectAltered = true;
            }

            if (ImGui::Button("Reset2"))
            {
                objectAltered = true;
                objectRotation = glm::vec3(0.0f);
            }

            ImGui::SameLine();

            if (ImGui::SliderFloat3("Rotação", glm::value_ptr(objectRotation), -180.0f, 180.0f, "%.0f"))
            {
                objectAltered = true;
            }

            if (ImGui::Button("Reset3"))
            {
                objectAltered = true;
                objectScale = 0.0f;
            }

            ImGui::SameLine();

            if (ImGui::SliderFloat("Escala", &objectScale, 0.0f, 5.0f, "%.3f"))
            {
                objectAltered = true;
            }



            if (ImGui::Button("Reset4"))
            {
                viewAltered = true;
                viewPosition = glm::vec3(0.0f);
            }

            ImGui::SameLine();

            if (ImGui::SliderFloat3("Posição camera", glm::value_ptr(viewPosition), -50.0f, 50.0f, "%.0f"))
            {
                viewAltered = true;
            }

            if (ImGui::Button("Reset5"))
            {
                viewAltered = true;
                viewTarget = glm::vec3(0.0f);
            }

            ImGui::SameLine();

            if (ImGui::SliderFloat3("Alvo camera", glm::value_ptr(viewTarget), -50.0f, 50.0f, "%.0f"))
            {
                viewAltered = true;
            }

            ImGui::End();
            ///////////////

            if (objectAltered)
            {
                modelMat = setModelMat(objectPosition, objectRotation, objectScale);
                material.setGlmMat4("u_modelMat", modelMat);
                objectAltered = false;
            }

            if (viewAltered)
            {
                viewMat = setViewMat(viewPosition, viewTarget, viewUp);
                material.setGlmMat4("u_viewMat", viewMat);
                viewAltered = false;
            }


            if (matrixAltered)
            {
                time = properTime = 0.0f;
                physics.reset();
                generator = buildPoincare(linearAccel, angularAccel, glm::vec4(0.0f));
                physics.setCenter(objCenter, mat);
                physics.setGroupGeneratorMat(generator, dq, MOTION::inertial);
                dTime = physics.getExternTimeInterval();
                dProperTime = physics.getProperTimeInterval();
                matrixAltered = false;
                isPaused = true;
            }

            if (!isPaused)
            {
                time += dTime;
                properTime += dProperTime;
            }


            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            render(mesh, physics, material);

            //physics.updatePoincareGroupMat();
            
            window.renderImGui();

			window.swapBuffers();
		}
	}

	catch(std::exception& e)
	{
		std::cout << "exception caught: " << e.what() << std::endl;
	}

	return 0;
}

glm::mat4 setModelMat(glm::vec3& translation, glm::vec3& rotation, float scale)
{
    glm::mat4 modelMat(1.0f);
    modelMat = glm::translate(modelMat, translation);
    modelMat = glm::rotate(modelMat, glm::radians(rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMat = glm::rotate(modelMat, glm::radians(rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMat = glm::rotate(modelMat, glm::radians(rotation[2]), glm::vec3(0.0f, 0.0f, 2.0f));
    modelMat = glm::scale(modelMat, glm::vec3(scale, scale, scale));
    return modelMat;
}

glm::mat4 setViewMat(glm::vec3& position, glm::vec3& target, glm::vec3& up)
{
    return glm::lookAt(position, target, up);
}


void initialize(glm::mat4& modelMat, glm::mat4& viewMat, glm::mat4& projectionMat, Window& window)
{
    modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, -5.0f));
    modelMat = glm::rotate(modelMat, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMat = glm::scale(modelMat, glm::vec3(0.25f, 0.25f, 0.25f));
    projectionMat = glm::perspective(glm::radians(45.0f), window.getAspectRatio(), 0.1f, 100.0f);
}
