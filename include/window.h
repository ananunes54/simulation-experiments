#ifndef WINDOW_INCLUDED_H
#define WINDOW_INCLUDED_H

#include <GLFW/glfw3.h>

class Window
{
    private:
        GLFWwindow* m_handle = nullptr;
        unsigned int mWidth;
        unsigned int mHeight;
        int m_fbWidth = 0;
        int m_fbHeight = 0;
        float m_aspectRatio = 1.0f;
        void updateViewportAndRatio(int width, int heigth);
        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
        static void contentScaleCallback(GLFWwindow* window, float xscale, float yscale);

    public:
        Window(unsigned int width, unsigned int height);
        Window(int width, int height, const char* title);
        ~Window();
        void makeCurrent();
        bool shouldClose();
        void swapBuffers();
        static void pollEvents();
        float getAspectRatio();
        int getFramebufferWidth();
        int getFramebufferHeight();
        GLFWwindow* getHandle();
};

#endif
