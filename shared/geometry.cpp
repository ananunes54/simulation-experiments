#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <cstdlib>

#include <glm/glm.hpp>

#include <geometry.h>


Geometry::Geometry(const char* geometrySource)
{
    std::ifstream objFile(geometrySource);
    if (objFile.is_open() == false)
    {
        std::cout << "could not open file: " << geometrySource << std::endl;
        return;
    }

    std::vector<glm::vec3> tempVertices(1, glm::vec3(0.0f));
    std::vector<glm::vec3> tempNormals(1, glm::vec3(0.0f));
    std::vector<glm::vec2> tempTextures(1, glm::vec2(0.0f));
    std::unordered_map<VertexKey, unsigned int, VertexKeyHash> uniqueVertices;

    std::string line;
    while(std::getline(objFile, line, '\n'))
    {
        std::stringstream chunks(line);
        std::string type;
        chunks >> type;
        if (type == "v")
        {
            glm::vec3 vertex;
            chunks >> vertex.x >> vertex.y >> vertex.z;
            tempVertices.push_back(vertex);
        }

        else if (type == "vn")
        {
            glm::vec3 normal;
            chunks >> normal.x >> normal.y >> normal.z;
            tempNormals.push_back(normal);
        }

        else if (type == "vt")
        {
            glm::vec2 texture;
            chunks >> texture.x >> texture.y;
            tempTextures.push_back(texture);
        }

        else if (type == "f" || type == "l")
        {
            std::string vertexData;
            while (chunks >> vertexData)
            {
                VertexKey key = {0, 0, 0};
                const char* vertexDataPtr = vertexData.c_str();
                char* endPtr;

                key.v = std::strtol(vertexDataPtr, &endPtr, 10);
                vertexDataPtr = endPtr + (*endPtr == '/');

                key.vt = std::strtol(vertexDataPtr, &endPtr, 10);
                vertexDataPtr = endPtr + (*endPtr == '/');
                
                key.vn = std::strtol(vertexDataPtr, &endPtr, 10);

                unsigned int index = m_verticesComplete.size();
                auto [iterator, inserted] = uniqueVertices.insert({key, index});

                if (inserted)
                {
                    Vertex tempVertex = {tempVertices[key.v],
                                         tempNormals[key.vn],
                                         tempTextures[key.vt]};

                    m_verticesComplete.push_back(tempVertex);
                }

                m_indices.push_back(iterator->second);
            }
        }
    }

    objFile.close();
}

unsigned int Geometry::getNumOfVertices()
{
    return m_vertices.size();
}

unsigned int Geometry::getNumOfCompleteVertices()
{
    return m_verticesComplete.size();
}

unsigned int Geometry::getNumOfIndices()
{
    return m_indices.size();
}

glm::vec3* Geometry::getVertices()
{
    return m_vertices.data();
}

Vertex* Geometry::getCompleteVertices()
{
    return m_verticesComplete.data();
}

unsigned int* Geometry::getIndices()
{
    return m_indices.data();
}

