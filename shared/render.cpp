#include <glad/glad.h>
#include <render.h>
#include <mesh.h>
#include <physics.h>
#include <shaders.h>

void render(Mesh& mesh, Physics& physics, Shader& shader)
{
    glBindVertexArray(mesh.getVAO());
    glUseProgram(shader.getID());

    glUniformMatrix3fv(shader.getMotionMatUniform(), 1, GL_FALSE, physics.getMotionMatPtr()); 
    glUniformMatrix3fv(shader.getRefChangeMatUniform(), 1, GL_FALSE, physics.getRefChangeMatPtr());
    glUniform1f(shader.getVelocityUniform(), physics.getVelocityMagnitude());
    glUniform1f(shader.getGammaUniform(), physics.getGamma());
    
    if (mesh.getPrimitive() == Primitive::line)
    {
        glDrawElements(GL_LINES, mesh.getIndicesCount(), GL_UNSIGNED_INT, 0);
    }
    else 
    {
        glDrawElements(GL_TRIANGLES, mesh.getIndicesCount(), GL_UNSIGNED_INT, 0);
    }
            
    glBindVertexArray(0);
}
