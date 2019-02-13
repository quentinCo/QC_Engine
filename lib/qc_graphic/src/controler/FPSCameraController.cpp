#define _USE_MATH_DEFINES
//#define NOMINMAX
#include <cmath>

#include<qc_graphic\controler\FPSCameraController.hpp>


using namespace qc;
using namespace qc::controller;

/*-------------------- FPSCAMERACONTROLLER CONSTRUCTORS ---------------------------------*/
FPSCameraController::FPSCameraController()
    : window(nullptr), camera(nullptr), linearSpeed(0), rotationSpeed(0), cursorPosition(0), rollingIsActive(true)
{}

FPSCameraController::FPSCameraController(Window& window, render::Camera& cam, float linearSpeed, float rotationSpeed)
    : window(&window), camera(&cam), linearSpeed(linearSpeed), rotationSpeed(rotationSpeed), cursorPosition(0), rollingIsActive(true)
{
    GLFWwindow* glfwWindow = this->window->getGLFWwindow();
    if(glfwWindow)
        glfwGetCursorPos(glfwWindow, &this->cursorPosition.x, &this->cursorPosition.y);
}

/*-------------------- FPSCAMERACONTROLLER  GETTERS / SETTERS ----------------------------*/
void FPSCameraController::setCamera(render::Camera& cam)
{
    this->camera = &cam;
}

float FPSCameraController::getLinearSpeed()
{
    return this->linearSpeed;
}

void FPSCameraController::setLinearSpeed(float speed)
{
    this->linearSpeed = speed;
}

float FPSCameraController::getRotationSpeed()
{
    return this->rotationSpeed;
}

void FPSCameraController::setRotationSpeed(float speed)
{
    this->rotationSpeed = speed;
}


/*-------------------- FPSCAMERACONTROLLER FUNCTIONS -------------------------------------*/
void FPSCameraController::update()
{
    if (this->window == nullptr || this->camera == nullptr)
        return;

    // Camera options
    this->updateOptions();

    // Camera translation
    this->updateTranslation();

    // Camera rotation
    this->updateRoation();
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
void FPSCameraController::updateOptions()
{
    GLFWwindow* glfwWindow = this->window->getGLFWwindow();
    
    this->rollingIsActive = (glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
}

void FPSCameraController::updateTranslation()
{
    GLFWwindow* glfwWindow = this->window->getGLFWwindow();
    render::Camera& camera = *this->camera;

    float frameDuration = this->window->getFrameDurationSecond();

    glm::vec3 position = camera.getPosition();

    const glm::vec3& frontVector = camera.getFrontVector();
    const glm::vec3& rightVector = camera.getRightVector();
    const glm::vec3& upVector = camera.getUpVector();

    glm::vec3 translation = glm::vec3(0);

    // Front - back
    if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
        translation += this->linearSpeed * frameDuration * frontVector;
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        translation -= this->linearSpeed * frameDuration * frontVector;
    }

    // Left - right
    if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
        translation -= this->linearSpeed * frameDuration * rightVector;
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
        translation += this->linearSpeed * frameDuration * rightVector;
    }

    // Up - down
    if (glfwGetKey(glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        translation += this->linearSpeed * frameDuration * upVector;
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        translation -= this->linearSpeed * frameDuration * upVector;
    }
    position += translation;
    camera.setPosition(position);
}


void FPSCameraController::updateRoation()
{
    GLFWwindow* glfwWindow = this->window->getGLFWwindow();

    if (this->rollingIsActive == false)
    {
        glfwGetCursorPos(glfwWindow, &this->cursorPosition.x, &this->cursorPosition.y);
        return;
    }

    render::Camera& camera = *this->camera;

    float frameDuration = this->window->getFrameDurationSecond();

    glm::vec3 angles = camera.getAngles();

    glm::dvec2 currentCursorPos;
    glfwGetCursorPos(glfwWindow, &currentCursorPos.x, &currentCursorPos.y);
    glm::dvec2 delta = currentCursorPos - this->cursorPosition;

    if (delta != glm::dvec2(0))
    {
        this->cursorPosition = currentCursorPos;
        angles.y -= static_cast<float>(delta.x * frameDuration * this->rotationSpeed);
        angles.x = static_cast<float>(glm::min(M_PI * 0.5, glm::max(-M_PI * 0.5, angles.x - delta.y * frameDuration * this->rotationSpeed)));

        camera.setAngles(angles);
    }
}

