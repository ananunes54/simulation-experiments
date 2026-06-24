#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <shaders.h>
#include <vector>
#include <utils.h>
#include <string>

Shader::Shader(std::string& vertexShaderPath, std::string& fragmentShaderPath) : m_vertexShaderPath(vertexShaderPath), m_fragmentShaderPath(fragmentShaderPath) 
{
    m_ID = Shader::createProgram();
    introspectProgram();
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_ID));
}

unsigned int Shader::compileShader(unsigned int shaderType, std::string& shaderSource)
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

unsigned int Shader::createProgram()
{
	unsigned int program = glCreateProgram();
	
	std::string vertexShaderSource = readFromFile(m_vertexShaderPath);
	std::string fragmentShaderSource = readFromFile(m_fragmentShaderPath);

	
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

	GLCall(glAttachShader(program, vertexShaderId));
	GLCall(glAttachShader(program, fragmentShaderId));
	
	GLCall(glLinkProgram(program));

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

	GLCall(glDetachShader(program, vertexShaderId));
	GLCall(glDetachShader(program, fragmentShaderId));

	GLCall(glDeleteShader(vertexShaderId));
	GLCall(glDeleteShader(fragmentShaderId));
	
	return program;

}

void Shader::introspectProgram()
{
    GLCall(glGetProgramiv(m_ID, GL_ACTIVE_UNIFORMS, &m_uniformsCount));
    m_uniforms.resize(m_uniformsCount);

    int maxNameLenght;
    GLCall(glGetProgramiv(m_ID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLenght));

    GLint size;

    for (auto i = 0; i < m_uniformsCount; i++)
    {
        m_uniforms[i].program = m_ID;
        m_uniforms[i].index = i;
        m_uniforms[i].name.assign(maxNameLenght, '\0');
        GLCall(glGetActiveUniform(m_ID, i, maxNameLenght, nullptr, &size, &(m_uniforms[i]).type, m_uniforms[i].name.data()));
    }
}

unsigned int Shader::getID()
{
    return m_ID;
}

int Shader::getUniformsCount()
{
    return m_uniformsCount;
}

std::vector<UniformInfo> Shader::getUniforms()
{
    return m_uniforms;
}
