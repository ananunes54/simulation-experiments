#include <iostream>

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <window.h>

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
