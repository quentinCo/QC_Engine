#include <Application.hpp>

#include <iostream>

#include <memory>

#include <string>
#include <chrono>

#include <qc_graphic\Window.hpp>
#include <qc_graphic\Useful.hpp>

static qc::Window window;

/*-------------------- APPLICATION  CONSTRUCTOR ----------------------------------*/

Application::Application()
{
    window          = qc::Window(1600, 900, "QC Engine");
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
    }

    return 0;
}