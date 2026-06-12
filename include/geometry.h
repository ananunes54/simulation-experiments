#ifndef GEOMETRY_INCLUDED_H
#define GEOMETRY_INCLUDED_H
#include <vector>
#include <glm/glm.hpp>

class Geometry
{
    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;
    glm::vec2 m_objCenter;

public:
    Geometry(std::vector<float> vertices, std::vector<unsigned int> indices, glm::vec2 objCenter) : m_vertices(vertices), m_indices(indices), m_objCenter(objCenter) {}
    unsigned int getNumOfVertices();
    unsigned int getNumOfIndices();
    float* getVertices();
    unsigned int* getIndices();
};

#endif
