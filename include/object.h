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


class Object
{
private:
	Geometry m_geometry;
    Physics m_physics;
	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int m_ebo;
	int m_program;
	
	int m_velocityUniform;
	int m_gammaUniform;
	int m_motionMatUniform;
	int m_refChangeMatUniform;

public:
	Object(std::vector<float>& vertices, std::vector<unsigned int>& indices) : m_geometry(vertices, indices), m_physics() 
	{
		glGenBuffers(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		glBufferData(GL_ARRAY_BUFFER, m_geometry.getNumOfVertices() * sizeof(float), m_geometry.getVertices(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_geometry.getNumOfIndices() * sizeof(unsigned int), m_geometry.getIndices(), GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

    void setPrimitive(Primitive primitive)
    {
        m_geometry.setPrimitive(primitive);
    }


	void setAttribute(unsigned int location, unsigned int numOfComponents,  unsigned int strideInBytes, unsigned int offset)
	{
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		glVertexAttribPointer(location, numOfComponents, GL_FLOAT, GL_FALSE, strideInBytes, reinterpret_cast<void*>(static_cast<uintptr_t>(offset)));
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void setAccelerationMatrix(glm::mat3 accelerationMat, float dq)
	{
	    m_physics.setAccelerationMat(accelerationMat, dq);
	}

    void setFourPosition(glm::vec3 fourPosition)
    {
        m_physics.setFourPosition(fourPosition);
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
		glBindVertexArray(m_vao);
		
		if (m_program != -1)
		{
			glUseProgram(m_program);

			glUniformMatrix3fv(m_motionMatUniform, 1, GL_FALSE, glm::value_ptr(m_physics.getMotionMat()));
			glUniformMatrix3fv(m_refChangeMatUniform, 1, GL_FALSE, glm::value_ptr(m_physics.getRefChangeMat()));
			glUniform1f(m_velocityUniform, m_physics.getVelocityMagnitude());
			glUniform1f(m_gammaUniform, m_physics.getGamma());
		}

		if (m_geometry.getPrimitive() == Primitive::line)
		{
			glDrawElements(GL_LINES, m_geometry.getNumOfIndices(), GL_UNSIGNED_INT, m_geometry.getIndices());
		}

		else
		{
			glDrawElements(GL_TRIANGLES, m_geometry.getNumOfIndices(), GL_UNSIGNED_INT, m_geometry.getIndices());
		}

		m_physics.updateMotionMat();

		glBindVertexArray(0);
	}	
};

#endif
