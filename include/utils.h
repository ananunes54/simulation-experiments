#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED
#include <GLFW/glfw3.h>
#include <string>
#include <exception>
#include <glm/glm.hpp>

class FileException : public std::exception
{
private:
	std::string m_message;
public:
	explicit FileException(std::string& message) : m_message(message) {}
	const char* what() const noexcept override 
	{
		return m_message.c_str();
	}
};


class Glfw
{
public:
	Glfw();
	~Glfw();
};

class Window
{
private:
	GLFWwindow* handle;
	unsigned int mWidth;
	unsigned int mHeight;
public:
	Window(unsigned int width, unsigned int height);
	~Window();
	void makeCurrent();
	int shouldClose();
	void swapBuffers();

};

void loadGlad();

std::string readFromFile(std::string& fileName);

glm::mat3 exp(glm::mat3 mat);

glm::mat3 scale(glm::mat3 mat, float factor);

float minkowskiMetric(glm::vec3 v1, glm::vec3 v2);

#endif
