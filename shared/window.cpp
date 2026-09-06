#include <iostream>

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glfwSetWindowUserPointer(m_handle, this);
    glfwSetFramebufferSizeCallback(m_handle, framebufferSizeCallback);
    glfwSetWindowContentScaleCallback(m_handle, contentScaleCallback);

    glfwGetFramebufferSize(m_handle, &m_fbWidth, &m_fbHeight);
    updateViewportAndRatio(m_fbWidth, m_fbHeight);

    initImGui();
}

Window::~Window()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

	std::cout << "window destroyed." << std::endl;
	glfwDestroyWindow(m_handle);
    glfwTerminate();
}

void Window::initImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGui::StyleColorsLight();
    ImGui_ImplGlfw_InitForOpenGL(m_handle, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void Window::initImGuiFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Window::renderImGui()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Window::updateViewportAndRatio(int width, int height)
{
    if (height == 0)
        return;
    m_fbWidth = width;
    m_fbHeight = height;

    glViewport(0, 0, width, height);
    m_aspectRatio = (float)width / (float)height;
    m_ratioChanged = true;
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->updateViewportAndRatio(width, height);
}

// relacionado ao DPI
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

bool Window::ratioChanged()
{
    m_ratioChanged = !m_ratioChanged;
    return !m_ratioChanged;
}
