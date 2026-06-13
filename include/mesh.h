#ifndef MESH_INCLUDED_H
#define MESH_INCLUDED_H
#include <geometry.h>

enum class Primitive { triangle, line };

class Mesh
{
    private:
        Primitive m_primitive = Primitive::triangle;
        unsigned int m_vao;
        unsigned int m_indicesCount;
        unsigned int m_verticesCount;

    public:
        Mesh(Geometry geometry);
        void setPrimitive(Primitive primitive);
        Primitive getPrimitive();
        int getVAO();
        unsigned int getVerticesCount();
        unsigned int getIndicesCount();
};

#endif
