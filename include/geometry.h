#ifndef GEOMETRY_INCLUDED_H
#define GEOMETRY_INCLUDED_H
#include <vector>
#include <string>
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 vertex;
    glm::vec3 normal;
    glm::vec2 texture;
};

struct VertexKey
{
    float v, vt, vn;

    bool operator==(const VertexKey& other) const
    {
        return v==other.v && vt==other.vt && vn==other.vn; 
    }
};

struct VertexKeyHash
{
    std::size_t operator()(const VertexKey& key) const
    {
        return std::hash<int>()(key.v) ^ std::hash<int>()(key.vt) ^ std::hash<int>()(key.vn);
    }
};

class Geometry
{
    std::vector<glm::vec3> m_vertices;
    std::vector<Vertex> m_verticesComplete;
    std::vector<unsigned int> m_indices;
    glm::vec2 m_objCenter;

public:
    Geometry(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices, glm::vec2 objCenter) : m_vertices(vertices), m_indices(indices), m_objCenter(objCenter) {}
    Geometry(const char* geometrySource);
    unsigned int getNumOfVertices();
    unsigned int getNumOfIndices();
    glm::vec3* getVertices();
    unsigned int* getIndices();
};

#endif
