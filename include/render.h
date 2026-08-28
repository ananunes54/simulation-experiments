#ifndef RENDER_INCLUDED_H
#define RENDER_INCLUDED_H

#include <vector>

#include <mesh.h>
#include <physics.h>
#include <shaders.h>

struct SimulationState;
class Camera;

void render(Mesh& mesh, Physics& physics, Shader& shader, SimulationState& state, Camera& camera);

#endif
