#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <geometry.h>
#include <physics.h>
#include <mesh.h>

class Object
{
private:
	Geometry m_geometry;
    Physics m_physics;
	unsigned int m_vao;
	int m_program;
    Mesh m_mesh;
	
	int m_velocityUniform;
	int m_gammaUniform;
	int m_motionMatUniform;
	int m_refChangeMatUniform;

public:
	Object(std::vector<float>& vertices, std::vector<unsigned int>& indices, glm::vec2 objCenter) : m_geometry(vertices, indices, objCenter), m_physics(), m_mesh(m_geometry)
	{
        m_physics.setFourPosition(glm::vec3(objCenter[0], objCenter[1], 1.0f));
	}

    void setPrimitive(Primitive primitive)
    {
        m_mesh.setPrimitive(primitive);
    }

	void setAccelerationMatrix(glm::mat3 accelerationMat, float dq)
	{
	    m_physics.setAccelerationMat(accelerationMat, dq);
	}

    float getProperTimeInterval()
    {
        return m_physics.getProperTimeInterval();
    }

    float getExternTimeInterval()
    {
        return m_physics.getExternTimeInterval();
    }

	void setProgram(unsigned int program)
	{
		
		m_program = program;
		if (program != -1)
		{
			m_motionMatUniform = glGetUniformLocation(program, "u_motionMat");
			m_refChangeMatUniform = glGetUniformLocation(program, "u_refChangeMat");
			m_velocityUniform = glGetUniformLocation(program, "u_velocity");
			m_gammaUniform = glGetUniformLocation(program, "u_gamma");
		}

		else
		{
			m_motionMatUniform = -1;
			m_refChangeMatUniform = -1;
			m_velocityUniform = -1;
			m_gammaUniform = -1;
		}
	}

	void draw()
	{
		glBindVertexArray(m_mesh.getVAO());
		
		if (m_program != -1)
		{
			glUseProgram(m_program);

			glUniformMatrix3fv(m_motionMatUniform, 1, GL_FALSE, glm::value_ptr(m_physics.getMotionMat()));
			glUniformMatrix3fv(m_refChangeMatUniform, 1, GL_FALSE, glm::value_ptr(m_physics.getRefChangeMat()));
			glUniform1f(m_velocityUniform, m_physics.getVelocityMagnitude());
			glUniform1f(m_gammaUniform, m_physics.getGamma());
		}

		if (m_mesh.getPrimitive() == Primitive::line)
		{
			glDrawElements(GL_LINES, m_geometry.getNumOfIndices(), GL_UNSIGNED_INT, 0);
		}

		else
		{
			glDrawElements(GL_TRIANGLES, m_geometry.getNumOfIndices(), GL_UNSIGNED_INT, 0);
		}

		m_physics.updateMotionMat();

		glBindVertexArray(0);
	}	
};

#endif
