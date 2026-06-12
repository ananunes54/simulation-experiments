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
#include <shaders.h>

class Object
{
private:
	Geometry m_geometry;
    Physics m_physics;
	unsigned int m_vao;
	Shader m_shader;
    Mesh m_mesh;

public:
	Object(std::vector<float>& vertices, std::vector<unsigned int>& indices, glm::vec2 objCenter, std::string& vertexShaderPath, std::string& fragmentShaderPath) : m_geometry(vertices, indices, objCenter), m_physics(), m_mesh(m_geometry), m_shader(vertexShaderPath, fragmentShaderPath)
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

    unsigned int getShaderID()
    {
        return m_shader.getID();
    }

    int getShaderVelocityUniform()
    {
        return m_shader.getVelocityUniform();
    }

    int getShaderGammaUniform()
    {
        return m_shader.getGammaUniform();
    }

    int getShaderMotionMatUniform()
    {
        return m_shader.getMotionMatUniform();
    }

    int getShaderRefChangeMatUniform()
    {
        return m_shader.getRefChangeMatUniform();
    }

    int getShaderColorUniform()
    {
        return m_shader.getColorUniform();
    }

    int getShaderTimeUniform()
    {
        return m_shader.getTimeUniform();
    }

    int getShaderProperTimeUniform()
    {
        return m_shader.getProperTimeUniform();
    }


	void draw()
	{
		glBindVertexArray(m_mesh.getVAO());
		
        glUseProgram(m_shader.getID());

        glUniformMatrix3fv(m_shader.getMotionMatUniform(), 1, GL_FALSE, m_physics.getMotionMatPtr());
        glUniformMatrix3fv(m_shader.getRefChangeMatUniform(), 1, GL_FALSE, m_physics.getRefChangeMatPtr());
        glUniform1f(m_shader.getVelocityUniform(), m_physics.getVelocityMagnitude());
        glUniform1f(m_shader.getGammaUniform(), m_physics.getGamma());

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
