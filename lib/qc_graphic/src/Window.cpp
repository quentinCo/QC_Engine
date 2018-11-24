#include<qc_graphic\Window.hpp>

#include<iostream>

#include<qc_graphic\Useful.hpp>

using namespace qc;

/*-------------------- WINDOW  CONSTRUCTOR ----------------------------------*/
Window::Window()
    : Window(0, 0, "No Name")
{}

Window::Window(int width, int height, const std::string & name)
    : glwfWindow(nullptr), width(width), height(height), name(name), shouldClose(false),
        frameDuration(0), fps(0)
{}

Window::~Window()
{
    if(this->glwfWindow)
        glfwDestroyWindow(this->glwfWindow);
    
    glfwTerminate();
}


/*-------------------- WINDOW  GETTERS / SETTERS ----------------------------------*/
bool Window::getShouldClose()
{
    return this->shouldClose;
}

float Window::getFrameDuration()
{
    return this->frameDuration;
}

float Window::getFps()
{
    return this->fps;
}



/*-------------------- WINDOW  INITIALISATION ----------------------------------*/
static void HelperGLFWError(int error, const char* description)
{
    std::string errorMess = "[ GLFW ] ";
    errorMess += description;
    qc::Useful::printError(errorMess);
}

bool Window::init()
{
    std::cout << "Window compiled against GLFW " << GLFW_VERSION_MAJOR << " " << GLFW_VERSION_MINOR << " " << GLFW_VERSION_REVISION << std::endl;

    // Set callback for glfw errors
    glfwSetErrorCallback(HelperGLFWError);

    int res = glfwInit();
    if (res != GLFW_TRUE)
    {
        return false;
    }

    // Create glfw window
    GLFWwindow* glwfWindow = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
    if (glwfWindow == nullptr)
    {
        qc::Useful::printError("glfwCreateWindow return nullptr");
        return false;
    }

    // Make it current
    glfwMakeContextCurrent(glwfWindow);

    // Init Window properties
    this->glwfWindow = glwfWindow;

    this->initClock = std::chrono::system_clock::now();
    this->prevFrameClock = this->initClock;

    qc::Useful::printValid("Window INITIALISED");
    return true;
}


/*-------------------- WINDOW  UPDATE ----------------------------------*/
void Window::update()
{
    // Swap buffer (rendering)
    glfwSwapBuffers(this->glwfWindow);

    // Inputs
    glfwPollEvents();

    this->shouldClose = glfwWindowShouldClose(this->glwfWindow);

    // Time datas
    auto currentClock = std::chrono::system_clock::now();

    std::chrono::duration<float> elapseTime = currentClock - this->prevFrameClock;
    this->frameDuration                     = 1000 * elapseTime.count();
    this->fps                               = 1000 / this->frameDuration;
    this->prevFrameClock                    = currentClock;
}