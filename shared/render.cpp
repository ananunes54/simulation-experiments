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

    material.bind();

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
    material.unbind();
}
