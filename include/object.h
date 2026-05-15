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
	
	int m_movementMatUniform;
	glm::mat3 m_movementMat;
	glm::mat3 m_auxMovementMat;


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

	void setMovementMatrix(glm::mat3 movementMat)
	{
		m_movementMat = m_auxMovementMat = movementMat;
	}

	void setProgram(unsigned int program)
	{
		
		m_program = program;
		if (program != -1)
		{
			m_movementMatUniform = glGetUniformLocation(program, "u_lorentzMat");
		}

		else
		{
			m_movementMatUniform = -1;
		}
	}

	void draw()
	{
		glBindVertexArray(m_vao);
		
		if (m_program != -1)
		{
			glUseProgram(m_program);
			glUniformMatrix3fv(m_movementMatUniform, 1, GL_FALSE, glm::value_ptr(m_movementMat));
		}

		if (m_shape == Shape::line)
		{
			glDrawElements(GL_LINES, m_indices.size(), GL_UNSIGNED_INT, m_indices.data());
		}

		else
		{
			glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, m_indices.data());
		}

		m_movementMat = m_movementMat * m_auxMovementMat;

		glBindVertexArray(0);
	}

	

};

#endif
