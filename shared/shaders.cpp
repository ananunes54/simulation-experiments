#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <shaders.h>
#include <vector>
#include <utils.h>

static unsigned int compileShader(unsigned int shaderType, std::string& shaderSource)
{
	unsigned int shader = glCreateShader(shaderType);
	const char* str = shaderSource.c_str();
	if (str == nullptr)
	{
		return -1;
	}

	glShaderSource(shader, 1, &str, nullptr);
	glCompileShader(shader);

	int compilationStatus = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compilationStatus);

	if (compilationStatus == GL_FALSE)
	{
		int logSize = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

		std::vector<char> log(logSize);
		glGetShaderInfoLog(shader, logSize, &logSize, &log[0]);

		for (char c : log)
		{
			std::cout << c;
		}
		std::cout << std::endl;

		glDeleteShader(shader);
		return -1;
	}

	return shader;
}

unsigned int createProgram(std::string& vertexShaderPath, std::string& fragmentShaderPath)
{
	unsigned int program = glCreateProgram();
	
	std::string vertexShaderSource = readFromFile(vertexShaderPath);
	std::string fragmentShaderSource = readFromFile(fragmentShaderPath);

	
	unsigned int vertexShaderId = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
	if (vertexShaderId == -1)
	{
		std::cout << "nao foi possivel compilar o vertex shader" << std::endl;

	}

	unsigned int fragmentShaderId = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	if (fragmentShaderId == -1)
	{
		std::cout << "nao foi possivel compilar o fragment shader" << std::endl;
	}



	if (vertexShaderId == -1 || fragmentShaderId == -1)
	{
		return -1;
	}

	glAttachShader(program, vertexShaderId);
	glAttachShader(program, fragmentShaderId);
	
	glLinkProgram(program);

	int linkingStatus = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int *)&linkingStatus);
	if (linkingStatus == GL_FALSE)
	{
		int logSize = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);

		std::vector<char> log(logSize);
		glGetProgramInfoLog(program, logSize, &logSize, &log[0]);

		glDeleteProgram(program);
		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);

		return -1;
	}

	glValidateProgram(program);
	
	int validateStatus = 0;
	glGetProgramiv(program, GL_VALIDATE_STATUS, &validateStatus);
	if (validateStatus == GL_FALSE)
	{
		glDeleteProgram(program);
		glDetachShader(program, vertexShaderId);
		glDetachShader(program, fragmentShaderId);
		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);
		return -1;
	}

	glDetachShader(program, vertexShaderId);
	glDetachShader(program, fragmentShaderId);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
	
	return program;

}
