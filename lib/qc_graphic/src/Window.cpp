#include <qc_graphic\Window.hpp>

#include <iostream>
#include <assert.h>

#include <qc_graphic\Useful.hpp>

using namespace qc;

// https://stackoverflow.com/questions/16823372/forcing-machine-to-use-dedicated-graphics-card
// http://www.nvidia.com/object/LO_optimus_whitepapers.html
// https://community.amd.com/thread/169965
extern "C"
{
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

extern "C"
{
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}



// PUBLIC -----------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------

/*-------------------- WINDOW  CONSTRUCTOR ----------------------------------*/
Window::Window()
    : Window(0, 0, "No Name")
{}

Window::Window(int width, int height, const std::string& name)
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
bool Window::getShouldClose() const
{
    return this->shouldClose;
}

float Window::getFrameDuration() const
{
    return this->frameDuration;
}

float Window::getFps() const
{
    return this->fps;
}



/*-------------------- WINDOW  INITIALISATION ----------------------------------*/
static void HelperGLFWError(int error, const char* description)
{
    std::string errorMess = "[ GLFW ] ";
    errorMess += description;
    qc::Useful::PrintError(errorMess);
}

bool Window::init()
{
    std::cout << "Window compiled against GLFW " << GLFW_VERSION_MAJOR << "." << GLFW_VERSION_MINOR << "." << GLFW_VERSION_REVISION << std::endl;

    // Set callback for glfw errors
    glfwSetErrorCallback(HelperGLFWError);

    int res = glfwInit();
    if (res != GLFW_TRUE)
        return false;

    // Create glfw window
    res = this->initGlfwWindow();
    if (res == false)
        return false;


    // Succes Message
    std::string mess;
    mess += "Window \"";
    mess += this->name;
    mess += "\" INITIALISED";

    qc::Useful::PrintSucced(mess);

    // Make it current
    Window::MakeCurrent(this);

    // Init OpenGL
    res = initOpenGL();
    if (res == false)
        return false;
   
    // Init clocks
    this->initClocks();

    return true;
}

#if 0
bool Window::init(const Window& mother)
{
    if (mother.glwfWindow == nullptr)
    {
        qc::Useful::printError("mother.glwfWindow is nullptr");
        return false;
    }

    // Create glfw window
    bool res = this->initGlfwWindow();
    if (res == false)
        return false;

    // Init clocks
    this->initClocks();

    // Succes Message
    std::string mess;
    mess += "Secondary window \"";
    mess += this->name;
    mess += "\" INITIALISED with \"";
    mess += mother.name;
    mess += "\" as mother";

    qc::Useful::PrintSucced(mess, 1);
    return true;
}
#endif

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


// Statics ----------------------------------------------------------------------------------------------------------------------------

Window* Window::currentWindow = nullptr;

bool Window::MakeCurrent(Window* window)
{
    assert(window != nullptr);

    if (window->glwfWindow == nullptr)
    {
        std::string mess;
        mess += "\"";
        mess += window->name;
        mess += "\" can't be made as current, window.glwfWindow is nullptr";
        qc::Useful::PrintError(mess);
        return false;
    }

    glfwMakeContextCurrent(window->glwfWindow);

    Window::currentWindow = window;

    std::cout << "Window \"" << window->name << "\" is current" << std::endl;

    return true;
}

Window* Window::GetCurrent()
{
    return Window::currentWindow;
}









/*
 *
 *
 *
 *
 *
 */


// PRIVATE ----------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------

bool Window::initGlfwWindow()
{
    GLFWwindow* window = glfwCreateWindow(this->width, this->height, this->name.data(), NULL, NULL);
    if (window == nullptr)
    {
        qc::Useful::PrintError("glfwCreateWindow return nullptr");
        return false;
    }

    this->glwfWindow = window;
    return true;
}


bool Window::initOpenGL()
{
    if (gladLoadGL() == false)
    {
        Useful::PrintError("Glad is unable to load OpenGL");
        return false;
    }
    qc::Useful::PrintSucced("Glad initialised OpenGL with success");

    std::cout << "Rendering properties: " << std::endl;
    std::cout << "-- OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "-- GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "-- Graphic card: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "-- Renderer: " << glGetString(GL_RENDERER) << std::endl;

    return true;
}


void Window::initClocks()
{
    this->initClock = std::chrono::system_clock::now();
    this->prevFrameClock = this->initClock;
}
