#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <mesh.h>
#include <geometry.h>

Mesh::Mesh(Geometry geometry)
{
    unsigned int temp_vbo;
    unsigned int temp_ebo;
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &temp_vbo);
    glGenBuffers(1, &temp_ebo);
    
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, temp_vbo);
    glBufferData(GL_ARRAY_BUFFER, geometry.getNumOfVertices() * sizeof(float), geometry.getVertices(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry.getNumOfIndices() * sizeof(unsigned int), geometry.getIndices(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::setPrimitive(Primitive primitive)
{
    m_primitive = primitive;
}

Primitive Mesh::getPrimitive()
{
    return m_primitive;
}

int Mesh::getVAO()
{
    return m_vao;
}
