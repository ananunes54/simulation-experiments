#include <glad/glad.h>
#include <render.h>
#include <mesh.h>
#include <physics.h>
#include <shaders.h>
#include <glm/gtc/matrix_transform.hpp>
#include <utils.h>
#include <camera.h>

void render(Mesh& mesh, Physics& physics, Shader& shader, SimulationState& state, Camera& camera)
{
    GLCall(glBindVertexArray(mesh.getVAO()));

    if (!state.paused || state.stateAltered)
    {
        shader.setFloat("u_time", state.time);
        shader.setFloat("u_properTime", physics.getProperTime());
    }

    if (physics.physicsAltered)
    {
        shader.setGlmMat4("u_poincareGroupMat", physics.getPoincareGroupMat4());
        shader.setGlmVec4("u_poincareTranslationVec", physics.getPoincareTranslationVec());
        shader.setFloat("u_gamma", physics.getGamma());
        shader.setFloat("u_velocity", physics.getVelocityMagnitude());
        shader.setGlmMat4("u_refChangeMat", physics.getRefChangeMat()); 
        shader.setFloat("u_angVelocity", physics.getProperAngVelocity());
        shader.setGlmMat3("u_alignmentMat", physics.getAlignmentMat());
        shader.setGlmMat3("u_alignmentMatInverse", glm::inverse(physics.getAlignmentMat()));
    }

    if (camera.cameraAltered)
    {
        shader.setGlmMat4("u_viewMat", camera.getViewMat());
        shader.setGlmMat4("u_projectionMat", camera.getProjectionMat());
    }
    

    shader.setGlmVec4("u_color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    state.stateAltered = false;
    physics.physicsAltered = false;
    camera.cameraAltered = false;

    shader.bind();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (mesh.getPrimitive() == Primitive::line)
    {
        GLCall(glDrawElements(GL_LINES, mesh.getIndicesCount(), GL_UNSIGNED_INT, 0));
    }
    else 
    {
        GLCall(glDrawElements(GL_TRIANGLES, mesh.getIndicesCount(), GL_UNSIGNED_INT, 0));
    }
            
    GLCall(glBindVertexArray(0));
    shader.unbind();
}
