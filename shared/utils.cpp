#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <utils.h>
#include <iostream>
#include <fstream>
#include <system_error>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <matrix_exponential.h>

void sim::glClearErrors()
{
    while(glGetError() != GL_NO_ERROR);
}

void sim::glLog(const char* functionName, const char* file, int line)
{
    while(GLenum err = glGetError())
    {
        std::cout << "[OpenGL] Error: (" << err << ") " << "in function " << functionName << "in file " << file << std::endl;
    }
}


std::string readFromFile(std::string& fileName)
{
	std::ifstream file(fileName.data());
	if (!file) 
	{
		std::string msg("nao foi possivel carregar o arquivo.");
		throw FileException(msg);
	}
	
	file.exceptions(std::ifstream::eofbit | std::ifstream::failbit | std::ifstream::badbit);
	try 
	{
		
		file.seekg(0, std::ios::end);
		int fileSize = file.tellg();
		std::string fileContent(fileSize, ' ');
		file.seekg(0, std::ios::beg);
		file.read(fileContent.data(), fileSize);
		file.close();
		return fileContent;
	}
	catch(std::ios_base::failure& e)
	{
		if (file.is_open())
		{
			file.close();
		}

		std::cerr << "[EXCEÇÃO] Erro ao processar o shader '" << fileName << "'\n"
                  << "Detalhes: " << e.what() << "\n" << "Codigo: " << e.code() << std::endl;
		return "";
	}
}


glm::mat4 exp(glm::mat4 mat)
{
	double buffer[16];

	for (auto i = 0; i < 4; i++)
	{
		for (auto j = 0; j < 4; j++)
		{
			buffer[i*4 + j] = static_cast<double>(mat[i][j]);
		}
	}

	double* expPtr = r8mat_expm1(4, buffer);
	glm::mat4 tempMat(0.0f);
	
	for (auto i = 0; i < 4; i++)
	{
		for (auto j = 0; j < 4; j++)
		{
			tempMat[i][j] = static_cast<float>(expPtr[i*4 + j]);
		}
	}

    free(expPtr);
	return tempMat;
}

glm::mat4 scale(glm::mat4 mat, float factor)
{
    glm::mat4 tempMat(1.0f);

    for (auto i = 0; i < 4; i++)    
    {
        for (auto j = 0; j < 4; j++)
        {
            tempMat[j][i] = mat[j][i] * factor;        
        }
    }

    return tempMat;
}

void printMat(const float* sourceMat, unsigned matDimension, std::ostream& output)
{
    for (auto j = 0; j < matDimension; j++)
    {
        for (auto i = 0; i < matDimension; i++)
        {
            output << "[" << sourceMat[i*matDimension + j] << "]";
        }

        output << std::endl;
    }
}

void print(const glm::mat4& mat, std::ostream& output)
{
    for (auto j = 0; j < 4; j++)
    {
        for (auto i = 0; i < 4; i++)
        {
            output << "[" << mat[i][j] << "]";
        }

        output << std::endl;
    }

    output << std::endl;
}

void print(const glm::vec4& mat, std::ostream& output)
{
    for (auto j = 0; j < 4; j++)
    {
        output << "[" << mat[j] << "]";
    }

    output << std::endl;
    output << std::endl;
}

float minkowskiMetric(glm::vec4 v1, glm::vec4 v2)
{
	return v1[0] * v2[0] - v1[1] * v2[1] - v1[2] * v2[2] - v1[3] * v2[3];
}

