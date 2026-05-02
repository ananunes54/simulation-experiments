#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED
#include <GLFW/glfw3.h>

class Glfw
{
public:
	Glfw();
	~Glfw();
};

class Window
{
private:
	GLFWwindow* handle;
	unsigned int mWidth;
	unsigned int mHeight;
public:
	Window(unsigned int width, unsigned int height);
	~Window();
	void makeCurrent();
	int shouldClose();
	void swapBuffers();

};

void loadGlad();

#endif
