#include <glad/glad.h>
#include <render.h>
#include <mesh.h>
#include <physics.h>
#include <shaders.h>
#include <material.h>

void render(Mesh& mesh, Physics& physics, Material& material)
{
    glBindVertexArray(mesh.getVAO());
   
    std::string motionMat = "u_motionMat";
    material.setGlmMat3(motionMat, physics.getMotionMat());

    std::string gamma = "u_gamma";
    material.setFloat(gamma, physics.getGamma());

    std::string velocity = "u_velocity";
    material.setFloat(velocity, physics.getVelocityMagnitude());

    std::string refChangeMat = "u_refChangeMat";
    material.setGlmMat3(refChangeMat, physics.getRefChangeMat());

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
