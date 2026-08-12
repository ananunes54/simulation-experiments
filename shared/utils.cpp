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

void Glfw::loadGlad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::runtime_error("não foi possivel carregar o glad.");
	}
}

Window::Window(int width, int height, const char* title)
{
	#ifdef WAYLAND
	glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
	#endif

	if (!glfwInit())
	{
		const char* description;
		int error = glfwGetError(&description);
		throw std::runtime_error(description);
	}

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

	m_handle = glfwCreateWindow(width, height, title, NULL, NULL);

	if (!m_handle)
	{
		const char* description;
		int err = glfwGetError(&description);
        glfwTerminate();
		throw std::runtime_error(description);
	}

    glfwMakeContextCurrent(m_handle);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::runtime_error("não foi possivel carregar o glad.");
	}

    glfwSetWindowUserPointer(m_handle, this);
    glfwSetFramebufferSizeCallback(m_handle, framebufferSizeCallback);
    glfwSetWindowContentScaleCallback(m_handle, contentScaleCallback);

    glfwGetFramebufferSize(m_handle, &m_fbWidth, &m_fbHeight);
    updateViewportAndRatio(m_fbWidth, m_fbHeight);
}

Window::~Window()
{
	std::cout << "window destroyed." << std::endl;
	glfwDestroyWindow(m_handle);
    glfwTerminate();
}

void Window::updateViewportAndRatio(int width, int height)
{
    if (height == 0)
        return;
    m_fbWidth = width;
    m_fbHeight = height;

    glViewport(0, 0, width, height);
    m_aspectRatio = (float)width / (float)height;
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->updateViewportAndRatio(width, height);
}

void Window::contentScaleCallback(GLFWwindow* window, float xscale, float yscale)
{
    auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    std::cout << "new scale: " << xscale << ", " << yscale << std::endl;
}

void Window::makeCurrent()
{
	glfwMakeContextCurrent(m_handle);
}

bool Window::shouldClose()
{
	return glfwWindowShouldClose(m_handle);
}

void Window::swapBuffers()
{
	glfwSwapBuffers(m_handle);
}

void Window::pollEvents()
{
    glfwPollEvents();
}

float Window::getAspectRatio()
{
    return m_aspectRatio;
}

int Window::getFramebufferWidth()
{
    return m_fbWidth;
}

int Window::getFramebufferHeight()
{
    return m_fbHeight;
}

GLFWwindow* Window::getHandle()
{
    return m_handle;
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
	return v1[0] * v2[0] - v1[1] * v2[1];
}

