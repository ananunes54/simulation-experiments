#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <exception>
#define WAYLAND

class GLFW
{
public:
	GLFW()
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

	~GLFW()
	{
		std::cout << "terminated" << std::endl;
		glfwTerminate();
	}
};


int main ()
{
	try
	{
		GLFW glfw;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}
