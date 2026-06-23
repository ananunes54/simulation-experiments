#include <glad/glad.h>
#include <render.h>
#include <mesh.h>
#include <physics.h>
#include <shaders.h>
#include <material.h>
#include <glm/gtc/matrix_transform.hpp>

void render(Mesh& mesh, Physics& physics, Material& material)
{
    glBindVertexArray(mesh.getVAO());
   
    std::string motionMat = "u_motionMat";
    material.setGlmMat4(motionMat, physics.getMotionMat());

    std::string gamma = "u_gamma";
    material.setFloat(gamma, physics.getGamma());

    std::string velocity = "u_velocity";
    material.setFloat(velocity, physics.getVelocityMagnitude());

    std::string refChangeMat = "u_refChangeMat";
    material.setGlmMat4(refChangeMat, physics.getRefChangeMat());

    glm::mat4 modelMat(1.0f);
    modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, -3.0f));
    //modelMat = glm::rotate(modelMat, glm::radians(-15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    std::string model = "u_modelMat";
    material.setGlmMat4(model, modelMat);

    glm::mat4 viewMat(1.0f);
    viewMat = glm::rotate(viewMat, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    viewMat = glm::translate(viewMat, glm::vec3(0.0f, -1.0f, 0.0f));
    std::string view = "u_viewMat";
    material.setGlmMat4(view, viewMat);

    glm::mat4 projectionMat(1.0f);
    projectionMat = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    std::string projection = "u_projectionMat";
    material.setGlmMat4(projection, projectionMat);

    material.bind();

    if (mesh.getPrimitive() == Primitive::line)
    {
        glDrawElements(GL_LINES, mesh.getIndicesCount(), GL_UNSIGNED_INT, 0);
    }
    else 
    {
        glDrawElements(GL_TRIANGLES, mesh.getIndicesCount(), GL_UNSIGNED_INT, 0);
    }
            
    glBindVertexArray(0);
    material.unbind();
}
