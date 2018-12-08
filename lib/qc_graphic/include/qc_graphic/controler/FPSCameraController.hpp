#pragma once

#include <qc_graphic\controler\Controller.hpp>
#include <qc_graphic\render\Camera.hpp>
#include <qc_graphic\Window.hpp>

namespace qc
{

namespace controller
{

class FPSCameraController : public Controller
{

public:
    // Constructors
    FPSCameraController();
    FPSCameraController(Window& window, render::Camera& cam, float linearSpeed = 10, float rotationSpeed = 0.2);

    ~FPSCameraController() {}

    // Getters / Setters
    void setCamera(render::Camera& cam);

    float getLinearSpeed();
    void setLinearSpeed(float speed);

    float getRotationSpeed();
    void setRotationSpeed(float speed);


    // Functions
    virtual void update() override;

private:
    Window*         window;
    render::Camera* camera;

    float linearSpeed;
    float rotationSpeed;

    glm::dvec2 cursorPosition;

    // Function
    void updateTranslation();
    void updateRoation();
};

};  // ! namespace controller

};  // ! namespace qc