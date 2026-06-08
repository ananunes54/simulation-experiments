#ifndef GEOMETRY_INCLUDED_H
#define GEOMETRY_INCLUDED_H
#include <vector>

enum class Primitive { triangle, line };

class Geometry
{
    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;
    Primitive m_primitive = Primitive::triangle;

public:
    Geometry(std::vector<float> vertices, std::vector<unsigned int> indices) : m_vertices(vertices), m_indices(indices) {}
    Geometry(std::vector<float> vertices, std::vector<unsigned int> indices, Primitive primitive) : m_vertices(vertices), m_indices(indices), m_primitive(primitive) {}
    unsigned int getNumOfVertices();
    unsigned int getNumOfIndices();
    float* getVertices();
    unsigned int* getIndices();
    Primitive getPrimitive();
    
    void setPrimitive(Primitive primitive);
};

#endif
