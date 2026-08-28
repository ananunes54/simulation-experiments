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
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    glm::vec2 m_objCenter;
    unsigned int m_numEdges;

public:
    Geometry() {};
    unsigned int getNumOfVertices();
    unsigned int getNumOfIndices();
    Vertex* getVertices();
    unsigned int* getIndices();
    unsigned int getNumEdges();
    void setNumEdges(int numEdges);

    void pushBackVertex(const Vertex& vertex);
    void pushBackIndex(int index);
};

void parseObj(const char* geometrySource, Geometry& target);

#endif
