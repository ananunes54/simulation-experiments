#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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
            m_vertices.push_back(vertex);
            std::cout << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
        }
        else if (type == "vn")
        {
            glm::vec3 normal;
            chunks >> normal.x >> normal.y >> normal.z;
            m_normalVectors.push_back(normal);
            std::cout << normal.x << " " << normal.y << " " << normal.z << std::endl;
        }
        else if (type == "vt")
        {
            glm::vec2 texture;
            chunks >> texture.x >> texture.y;
            m_textureCoordinates.push_back(texture);
            std::cout << texture.x << " " << texture.y << std::endl;
        }
        else if (type == "f")
        {
        }
        else {}
    }

    objFile.close();
}

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

