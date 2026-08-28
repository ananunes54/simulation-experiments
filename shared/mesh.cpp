#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <mesh.h>
#include <geometry.h>
#include <utils.h>

void Mesh::createMesh(Geometry geometry)
{
    m_verticesCount = geometry.getNumOfVertices();
    m_indicesCount = geometry.getNumOfIndices();

    if (geometry.getNumEdges() == 2)    
        m_primitive = Primitive::line;
    else
        m_primitive = Primitive::triangle;

    unsigned int temp_vbo;
    unsigned int temp_ebo;
    GLCall(glGenVertexArrays(1, &m_vao));
    GLCall(glGenBuffers(1, &temp_vbo));
    GLCall(glGenBuffers(1, &temp_ebo));
    
    GLCall(glBindVertexArray(m_vao));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, temp_vbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, geometry.getNumOfVertices() * sizeof(glm::vec3), geometry.getVertices(), GL_STATIC_DRAW));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp_ebo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry.getNumOfIndices() * sizeof(unsigned int), geometry.getIndices(), GL_STATIC_DRAW));

    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0));
    GLCall(glEnableVertexAttribArray(0));

    GLCall(glBindVertexArray(0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void Mesh::createMeshFromObj(Geometry geometry)
{
    m_verticesCount = geometry.getNumOfVertices();
    m_indicesCount = geometry.getNumOfIndices();
    
    unsigned int temp_vbo;
    unsigned int temp_ebo;
    GLCall(glGenVertexArrays(1, &m_vao));
    GLCall(glGenBuffers(1, &temp_vbo));
    GLCall(glGenBuffers(1, &temp_ebo));
    
    GLCall(glBindVertexArray(m_vao));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, temp_vbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, geometry.getNumOfVertices() * sizeof(Vertex), geometry.getVertices(), GL_STATIC_DRAW));
    
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp_ebo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry.getNumOfIndices() * sizeof(unsigned int), geometry.getIndices(), GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));

    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)));
    
    GLCall(glEnableVertexAttribArray(2));
    GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(Vertex), (void*)offsetof(Vertex, texture)));

    GLCall(glBindVertexArray(0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
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

unsigned int Mesh::getVerticesCount()
{
    return m_verticesCount;
}

unsigned int Mesh::getIndicesCount()
{
    return m_indicesCount;
}
