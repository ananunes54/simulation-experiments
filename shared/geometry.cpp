#include <geometry.h>

unsigned int Geometry::getNumOfVertices()
{
    return m_vertices.size();
}

unsigned int Geometry::getNumOfIndices()
{
    return m_indices.size();
}

float* Geometry::getVertices()
{
    return m_vertices.data();
}

unsigned int* Geometry::getIndices()
{
    return m_indices.data();
}

Primitive Geometry::getPrimitive()
{
    return m_primitive;
}

void Geometry::setPrimitive(Primitive primitive)
{
    m_primitive = primitive;
}
