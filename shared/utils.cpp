#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <utils.h>
#include <iostream>
#include <exception>
#include <fstream>
#include <system_error>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <matrix_exponential.h>

Glfw::Glfw()
{
	#ifdef WAYLAND
	glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
	#endif

	if (!glfwInit())
	{
		const char* description;
		int error = glfwGetError(&description);
		throw std::runtime_error(description);
	}
}

Glfw::~Glfw()
{
	std::cout << "glfw terminated." << std::endl;
	glfwTerminate();
}

Window::Window(unsigned int width, unsigned int height)
{
	#ifdef WAYLAND
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
	#endif

	handle = glfwCreateWindow(width, height, "window", NULL, NULL);

	if (!handle)
	{
		const char* description;
		int err = glfwGetError(&description);
		throw std::runtime_error(description);
	}
}

Window::~Window()
{
	std::cout << "window destroyed." << std::endl;
	glfwDestroyWindow(handle);
}

void Window::makeCurrent()
{
	glfwMakeContextCurrent(handle);
}

int Window::shouldClose()
{
	return glfwWindowShouldClose(handle);
}

void Window::swapBuffers()
{
	glfwSwapBuffers(handle);
}


void loadGlad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::runtime_error("não foi possivel carregar o glad.");
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


glm::mat3 exp(glm::mat3 m)
{
	double buffer[9];

	for (auto i = 0; i < 3; i++)
	{
		for (auto j = 0; j < 3; j++)
		{
			buffer[i*3 + j] = static_cast<double>(m[i][j]);
		}
	}

	double* expPtr = r8mat_expm1(3, buffer);
	glm::mat3 tempMat(0.0f);
	
	for (auto i = 0; i < 3; i++)
	{
		for (auto j = 0; j < 3; j++)
		{
			tempMat[i][j] = static_cast<float>(expPtr[i*3 + j]);
		}
	}

	return tempMat;
}
