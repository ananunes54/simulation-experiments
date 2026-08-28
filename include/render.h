#ifndef RENDER_INCLUDED_H
#define RENDER_INCLUDED_H

#include <vector>

#include <mesh.h>
#include <physics.h>
#include <shaders.h>

struct SimulationState;
class Camera;
class Transform;

class Render    
{
    uint32_t m_flags = ~CLEAR;

    public:
        enum Flag : uint32_t
        {   
            CLEAR = 0,
            PHYSICS = 1 << 0,
            TRANSFORM = 1 << 1,
            CAMERA = 1 << 2
        };

        void setFlag(Flag f);
        bool checkFlag(Flag f);
        void clearFlag(Flag f);
        void clearAllFlags();
        void render(Mesh& mesh, Physics& physics, Shader& shader, SimulationState& state, Camera& camera, Transform& model);
};


#endif
