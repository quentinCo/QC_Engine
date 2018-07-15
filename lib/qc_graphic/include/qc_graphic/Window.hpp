#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

namespace qc_graphic
{

class Window
{
public:
    Window(size_t w = 1280, size_t h = 720);
    Window(size_t w, size_t h, std::string name);

    ~Window();

    size_t getWidth()
    {
        return width;
    }

    size_t getHeight()
    {
        return height;
    }

    GLFWwindow* getGLFWwindow()
    {
        return glfwWindow;
    }

    bool shouldClose();
    void swapBuffer();

private:
    size_t      width;
    size_t      height;
    GLFWwindow* glfwWindow;


    void initGLFW();
    void initWindow(int w, int h, std::string name);
    void initOpenGL();
};

}// qc_graphic