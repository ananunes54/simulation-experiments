#include <glad/glad.h>
#include <render.h>
#include <mesh.h>
#include <physics.h>
#include <shaders.h>
#include <glm/gtc/matrix_transform.hpp>
#include <utils.h>
#include <camera.h>
#include <transform.h>
#include <control.h>
#include <iostream>

void Render::render(Mesh& mesh, Physics& physics, Shader& shader, SimulationState& state, Camera& camera, Transform& model)
{
    GLCall(glBindVertexArray(mesh.getVAO()));

    if (!state.paused || state.stateAltered)
    {
        shader.setFloat("u_time", state.time);
        shader.setFloat("u_properTime", physics.getProperTime());
    }

    if (m_flags & PHYSICS)
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

    if (m_flags & CAMERA)
    {
        shader.setGlmMat4("u_viewMat", camera.getViewMat());
        shader.setGlmMat4("u_projectionMat", camera.getProjectionMat());
    }

    if (m_flags & TRANSFORM)
    {
        shader.setGlmMat4("u_modelMat", model.getModelMat());
    }
    

    shader.setGlmVec4("u_color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    state.stateAltered = false;
    m_flags &= CLEAR;

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


void Render::setFlag(Flag f)
{
    m_flags |= f;
}

bool Render::checkFlag(Flag f)
{
    return m_flags & f;
}

void Render::clearFlag(Flag f)
{
    m_flags &= ~f;
}

void Render::clearAllFlags()
{
    m_flags = CLEAR;
}
