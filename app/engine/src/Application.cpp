#include <Application.hpp>

#include <iostream>

#include <memory>

#include <string>
#include <chrono>

#include <qc_graphic\Window.hpp>
#include <qc_graphic\Useful.hpp>
#include <qc_graphic\render\Camera.hpp>
#include <qc_graphic\controler\FPSCameraController.hpp>

static qc::Window window;
static qc::render::Camera camera;
static qc::controller::Controller* cameraController;
static qc::controller::FPSCameraController fpsCameraController;

/*-------------------- APPLICATION  CONSTRUCTOR ----------------------------------*/

Application::Application()
{
    window          = qc::Window(1600, 900, "QC Engine");

    fpsCameraController = qc::controller::FPSCameraController(window, camera);
    cameraController = &fpsCameraController;
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

        cameraController->update();
    }

    return 0;
}