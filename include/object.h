#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

enum class Shape : unsigned char
{
	personalized, line, triangle, rectangle, circle
};


class Object
{
private:
	std::vector<float> m_vertices;
	std::vector<unsigned int> m_indices;
	Shape m_shape = Shape::personalized;
	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int m_ebo;
	int m_program;
	float m_velocity = 0;
	float m_gamma = 1;
	
	int m_velocityUniform;
	int m_gammaUniform;
	int m_motionMatUniform;
	int m_refChangeMatUniform;
	glm::mat3 m_refChangeMat;
	glm::mat3 m_motionMat;
	glm::mat3 m_auxMotionMat;


public:
	Object(Shape objShape, std::vector<float>& vertices, std::vector<unsigned int>& indices) : m_vertices(vertices), m_indices(indices), m_shape(objShape)
	{
		glGenBuffers(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
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

	void setMotionMatrix(glm::mat3 motionMat)
	{
		m_motionMat = m_auxMotionMat = motionMat;
	}

	void setRefChangeMatrix(glm::mat3 refChangeMat)
	{
		m_refChangeMat = refChangeMat;
	}

	void setVelocity(float velocity)
	{
		m_velocity = velocity;
		m_gamma = 1 / sqrt(1 - pow(m_velocity, 2));
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
			glUniformMatrix3fv(m_motionMatUniform, 1, GL_FALSE, glm::value_ptr(m_motionMat));
			glUniformMatrix3fv(m_refChangeMatUniform, 1, GL_FALSE, glm::value_ptr(m_refChangeMat));
			glUniform1f(m_velocityUniform, m_velocity);
			glUniform1f(m_gammaUniform, m_gamma);
		}

		if (m_shape == Shape::line)
		{
			glDrawElements(GL_LINES, m_indices.size(), GL_UNSIGNED_INT, m_indices.data());
		}

		else
		{
			glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, m_indices.data());
		}

		m_motionMat = m_motionMat * m_auxMotionMat;

		glBindVertexArray(0);
	}

	

};

#endif
