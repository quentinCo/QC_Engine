#include <Application.hpp>

#include <iostream>

#include <memory>

#include <GLFW/glfw3.h>

#include <string>
#include <chrono>

#include <qc_graphic\Useful.hpp>
#include <qc_graphic\Window.hpp>

static qc::Window window;


/*-------------------- APPLICATION  CONSTRUCTOR ----------------------------------*/

Application::Application()
{
    window = qc::Window(1200, 480, "QC Engine");
}

Application::~Application()
{}

/*-------------------------------  RUN  ------------------------------------------*/

int Application::run()
{
    bool isInit = window.init();
    if (isInit == false)
    {
        return 0;
    }

    while (window.getShouldClose() == false)
    {
        window.update();
        qc::Useful::printValid(window.getFrameDuration());
    }

    return 0;
}