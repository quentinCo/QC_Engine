#pragma once

#include <qc_graphic/Camera.hpp>
#include <qc_graphic/Window.hpp>

namespace app
{

namespace controller
{

class CameraController
{

public:
    CameraController()
        :camera(nullptr), window(nullptr)
    {}

    CameraController(qc_graphic::Camera& camera, qc_graphic::Window& window);

    virtual void updateController(float elaspedTime);

private:
    qc_graphic::Camera * camera;
    qc_graphic::Window * window;

    bool                rotate;

    glm::dvec2          prevCursorPosition;

    void displacement(float elaspedTime);
    void rotation(float elaspedTime);

};

}// ! controller

}// ! app
