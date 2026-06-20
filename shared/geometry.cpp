#include <geometry.h>

unsigned int Geometry::getNumOfVertices()
{
    return m_vertices.size();
}

unsigned int Geometry::getNumOfIndices()
{
    return m_indices.size();
}

glm::vec3* Geometry::getVertices()
{
    return m_vertices.data();
}

unsigned int* Geometry::getIndices()
{
    return m_indices.data();
}

