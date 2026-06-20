#ifndef GEOMETRY_INCLUDED_H
#define GEOMETRY_INCLUDED_H
#include <vector>
#include <glm/glm.hpp>

class Geometry
{
    std::vector<glm::vec3> m_vertices;
    std::vector<unsigned int> m_indices;
    glm::vec2 m_objCenter;

public:
    Geometry(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices, glm::vec2 objCenter) : m_vertices(vertices), m_indices(indices), m_objCenter(objCenter) {}
    unsigned int getNumOfVertices();
    unsigned int getNumOfIndices();
    glm::vec3* getVertices();
    unsigned int* getIndices();
};

#endif
