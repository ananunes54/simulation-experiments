#include <glad/glad.h>
#include <render.h>
#include <mesh.h>
#include <physics.h>
#include <shaders.h>
#include <material.h>
#include <glm/gtc/matrix_transform.hpp>
#include <utils.h>

void render(Mesh& mesh, Physics& physics, Material& material)
{
    GLCall(glBindVertexArray(mesh.getVAO()));

    glm::mat4 modelMat(1.0f);
    modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, -3.0f));
    //modelMat = glm::rotate(modelMat, glm::radians(-15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    material.setGlmMat4("u_modelMat", modelMat);

    glm::mat4 viewMat(1.0f);
    viewMat = glm::rotate(viewMat, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    viewMat = glm::translate(viewMat, glm::vec3(0.0f, -1.0f, 0.0f));
    material.setGlmMat4("u_viewMat", viewMat);

    glm::mat4 projectionMat(1.0f);
    projectionMat = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    material.setGlmMat4("u_projectionMat", projectionMat);

    material.bind();

    if (mesh.getPrimitive() == Primitive::line)
    {
        GLCall(glDrawElements(GL_LINES, mesh.getIndicesCount(), GL_UNSIGNED_INT, 0));
    }
    else 
    {
        GLCall(glDrawElements(GL_TRIANGLES, mesh.getIndicesCount(), GL_UNSIGNED_INT, 0));
    }
            
    GLCall(glBindVertexArray(0));
    material.unbind();
}
