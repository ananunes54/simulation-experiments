#ifndef MESH_INCLUDED_H
#define MESH_INCLUDED_H
#include <geometry.h>

enum class Primitive { triangle, line };

class Mesh
{
    private:
        Primitive m_primitive = Primitive::triangle;
        unsigned int m_vao;

    public:
        Mesh(Geometry geometry);
        void setPrimitive(Primitive primitive);
        Primitive getPrimitive();
        int getVAO();
};

#endif
