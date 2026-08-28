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

struct SimulationState
{
    bool paused = true;
    bool stateAltered = false;
    float dTime = 0.01;
    float time = 0.0f;

    void reset()
    {
        paused = true;
        stateAltered = false;
        dTime = 0.01;
        time = 0.0f;
    }
};

std::string readFromFile(std::string& fileName);

glm::mat4 exp(glm::mat4 mat);

glm::mat4 scale(glm::mat4 mat, float factor);

void printMat(const float* sourceMat, unsigned matDimension, std::ostream& output);

void print(const glm::mat4& mat, std::ostream& output);

void print(const glm::vec4& v, std::ostream& output);

float minkowskiMetric(glm::vec4 v1, glm::vec4 v2);

#endif
