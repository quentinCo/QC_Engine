#define _USE_MATH_DEFINES
#define NOMINMAX
#include <cmath>

#include <algorithm>

#include <Controller/CameraController.hpp>

using namespace app::controller;

CameraController::CameraController(qc_graphic::Camera& camera, qc_graphic::Window& window)
    : camera(&camera), window(&window), rotate(false), prevCursorPosition(0.0)
{}

void CameraController::updateController(float elaspedTime)
{
    if (camera == nullptr || window == nullptr)
    {
        return;
    }

    displacement(elaspedTime);
    rotation(elaspedTime);
}

void CameraController::displacement(float elaspedTime)
{
    GLFWwindow* glfwWindow                          = window->getGLFWwindow();
    const glm::vec3& frontVector                    = camera->getFrontVector();
    const glm::vec3& leftVector                     = camera->getLeftVector();
    qc_graphic::geometry::Transformation& transform = camera->getTransfomation();

    float speedMove                                 = 5; // TODO Change it. Put speed information in a specific class
    glm::vec3 translation                           = glm::vec3(0);
    
    if (glfwGetKey(glfwWindow, GLFW_KEY_W))
        translation += speedMove * elaspedTime * frontVector;
    if (glfwGetKey(glfwWindow, GLFW_KEY_S))
        translation -= speedMove * elaspedTime * frontVector;

    if (glfwGetKey(glfwWindow, GLFW_KEY_A))
        translation += speedMove * elaspedTime * leftVector;
    if (glfwGetKey(glfwWindow, GLFW_KEY_D))
        translation -= speedMove * elaspedTime * leftVector;

    if (glfwGetKey(glfwWindow, GLFW_KEY_SPACE))
        translation += speedMove * elaspedTime * glm::vec3(0, 1, 0);
    if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_SHIFT))
        translation -= speedMove * elaspedTime * glm::vec3(0, 1, 0);

    if (translation != glm::vec3(0))
    {
        transform.translate(translation);
    }
}

void CameraController::rotation(float elaspedTime)
{
    GLFWwindow* glfwWindow                          = window->getGLFWwindow();
    qc_graphic::geometry::Transformation& transform = camera->getTransfomation();

    float speedRotation                             = 0.1;

    if (glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        rotate = true;
        glfwGetCursorPos(glfwWindow, &prevCursorPosition.x, &prevCursorPosition.y);
    }

    if (glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        rotate = false;

    if (rotate)
    {
        glm::dvec2 currentCursorPostion;
        glm::dvec2 deltaCursor;
        glfwGetCursorPos(glfwWindow, &currentCursorPostion.x, &currentCursorPostion.y);
        
        deltaCursor = prevCursorPosition - currentCursorPostion;
        prevCursorPosition = currentCursorPostion;

        glm::vec3 rotation = { 0,0,0 };

        rotation.y += static_cast<float>(deltaCursor.x * speedRotation * elaspedTime);
        rotation.x += static_cast<float>(deltaCursor.y * speedRotation * elaspedTime);

        transform.rotate(rotation);

        glm::vec3 orientation = transform.getRotation();
        orientation.x = std::min(static_cast<float>(M_PI * 0.5), std::max(static_cast<float>(-M_PI * 0.5), orientation.x));
            
        transform.setRotate(orientation);
    }
}
