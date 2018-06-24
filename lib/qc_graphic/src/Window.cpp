#include <qc_graphic/Window.hpp>

#include <iostream>


using namespace qc_graphic;

Window::Window(size_t w, size_t h)
    : Window(w,h,"Unnamed Window")
{}

Window::Window(size_t w, size_t h, std::string name)
{
    initGLFW();
    initWindow(w, h, name);
    initOpenGL();
    
    width   = w;
    height  = h;
}

Window::~Window()
{
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
}

bool Window::shouldClose()
{
    return !glfwWindowShouldClose(glfwWindow);
}

void Window::swapBuffer()
{
    // Swap front and back buffers
    glfwSwapBuffers(glfwWindow);
}

void Window::initGLFW()
{
    if (!glfwInit())
    {
        std::string errorMess = "Unable to init GLFW";
        std::cerr << errorMess.c_str() << std::endl;
        throw std::runtime_error(errorMess.c_str());
    }
}

void Window::initWindow(int w, int h, std::string name)
{
    glfwWindow = glfwCreateWindow(static_cast<int>(w), static_cast<int>(h), name.c_str(), nullptr, nullptr);

    if (!glfwWindow)
    {
        std::string errorMess = "Unable to create window";
        std::cerr << errorMess.c_str() << std::endl;
        throw std::runtime_error(errorMess.c_str());
    }

    // Make window's context
    glfwMakeContextCurrent(glfwWindow);
}

void Window::initOpenGL()
{
    if (!gladLoadGL())
    {
        std::string errorMess = "Unable to init OpenGL";
        std::cerr << errorMess.c_str() << std::endl;
        throw std::runtime_error(errorMess.c_str());
    }
}