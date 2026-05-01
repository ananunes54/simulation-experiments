#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <exception>
#define WAYLAND

class Glfw
{
public:
	Glfw()
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

	~Glfw()
	{
		std::cout << "glfw terminated." << std::endl;
		glfwTerminate();
	}
};

class Window
{
private:
	GLFWwindow* handle;
	unsigned int mWidth;
	unsigned int mHeight;
public:
	Window(unsigned int width, unsigned int height)
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

	~Window()
	{
		std::cout << "window destroyed." << std::endl;
		glfwDestroyWindow(handle);
	}

	void makeCurrent()
	{
		glfwMakeContextCurrent(handle);
	}

	int shouldClose()
	{
		return glfwWindowShouldClose(handle);
	}

	void swapBuffers()
	{
		glfwSwapBuffers(handle);
	}

};

void loadGlad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::runtime_error("não foi possivel carregar o glad.");
	}
}


int main ()
{
	try
	{
		Glfw glfw;
		Window window1(720, 720);
		
		window1.makeCurrent();
		loadGlad();

		while (!window1.shouldClose())
		{
			glClear(GL_COLOR_BUFFER_BIT);

			window1.swapBuffers();

			glfwPollEvents();
		}
	}

	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}
