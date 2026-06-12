#ifndef GEOMETRY_INCLUDED_H
#define GEOMETRY_INCLUDED_H
#include <vector>
#include <glm/glm.hpp>

enum class Primitive { triangle, line };

class Geometry
{
    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;
    glm::vec2 m_objCenter;
    Primitive m_primitive = Primitive::triangle;

public:
    Geometry(std::vector<float> vertices, std::vector<unsigned int> indices, glm::vec2 objCenter) : m_vertices(vertices), m_indices(indices), m_objCenter(objCenter) {}
    Geometry(std::vector<float> vertices, std::vector<unsigned int> indices, glm::vec2 objCenter, Primitive primitive) : m_vertices(vertices), m_indices(indices), m_primitive(primitive), m_objCenter(objCenter) {}
    unsigned int getNumOfVertices();
    unsigned int getNumOfIndices();
    float* getVertices();
    unsigned int* getIndices();
    Primitive getPrimitive();
    
    void setPrimitive(Primitive primitive);
};

#endif
