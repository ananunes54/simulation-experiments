#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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
            std::cout << "encontrou um vertice" << std::endl;
        }
        else if (type == "vt")
        {
            std::cout << "encontrou coordenada de textura" << std::endl;
        }
        else if (type == "vn")
        {
            std::cout << "encontrou um vetor normal de vertice" << std::endl;
        }
        else if (type == "f")
        {
            std::cout << "encontrou uma face poligonal" << std::endl;
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

