#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED
#include <GLFW/glfw3.h>
#include <string>
#include <exception>
#include <glm/glm.hpp>

#define GLCall(x) sim::glClearErrors; x; sim::glLog(#x, __FILE__, __LINE__);

namespace sim
{
    void glClearErrors();
    void glLog(const char* functionName, const char* file, int line);
};

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
    void loadGlad();
};

class Window
{
private:
	GLFWwindow* m_handle = nullptr;
	unsigned int mWidth;
	unsigned int mHeight;
    int m_fbWidth = 0;
    int m_fbHeight = 0;
    float m_aspectRatio = 1.0f;
    void updateViewportAndRatio(int width, int heigth);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void contentScaleCallback(GLFWwindow* window, float xscale, float yscale);

public:
	Window(unsigned int width, unsigned int height);
	Window(int width, int height, const char* title);
	~Window();
	void makeCurrent();
	bool shouldClose();
	void swapBuffers();
    static void pollEvents();
    float getAspectRatio();
    int getFramebufferWidth();
    int getFramebufferHeight();
    GLFWwindow* getHandle();

};

std::string readFromFile(std::string& fileName);

glm::mat4 exp(glm::mat4 mat);

glm::mat4 scale(glm::mat4 mat, float factor);

void printMat(const float* sourceMat, unsigned matDimension, std::ostream& output);

void print(const glm::mat4& mat, std::ostream& output);

void print(const glm::vec4& v, std::ostream& output);

float minkowskiMetric(glm::vec4 v1, glm::vec4 v2);

#endif
