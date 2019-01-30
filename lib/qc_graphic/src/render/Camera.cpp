#define _USE_MATH_DEFINES
//#define NOMINMAX
#include <cmath>

#include <iostream>

#include <glm\gtc\matrix_transform.hpp>

#include <qc_graphic\render\Camera.hpp>

using namespace qc::render;


// PUBLIC -----------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------

/*-------------------- CAMERA  CONSTRUCTOR ----------------------------------*/
Camera::Camera(const glm::vec3& pos, float fov, float near, float far, const glm::vec3& vFront)
    : position(pos), frontVector(vFront), rightVector(0), upVector(0), angles(0), viewMatrix(0),
        fov(fov), nearPlan(near), farPlan(far), projMatrix(0), viewportSize(0),
        isValidVector(false), isValidViewMatrix(false), isValidProjMatrix(false)
{
    this->computeAngles();
    this->computeVectors();
    this->computeViewMatrix();
}

Camera::Camera(const glm::vec3& pos, const glm::vec3& target, float fov, float near, float far)
    : position(pos), frontVector(0), rightVector(0), upVector(0), angles(0), viewMatrix(0),
        fov(fov), nearPlan(near), farPlan(far), projMatrix(0), viewportSize(0),
        isValidVector(false), isValidViewMatrix(false), isValidProjMatrix(false)
{
    this->frontVector = target - pos;
    glm::normalize(this->frontVector);

    this->computeAngles();
    this->computeVectors();
    this->computeViewMatrix();
}


/*-------------------- CAMERA  GETTERS / SETTERS ----------------------------*/
const glm::vec3& Camera::getPosition()
{
    return this->position;
}

void Camera::setPosition(const glm::vec3& pos)
{
    this->position          = pos;
    this->isValidViewMatrix = false;
}

const glm::vec3& Camera::getFrontVector()
{
    if (this->isValidVector == false)
    {
        this->computeVectors();
        this->computeViewMatrix();
    }

    return this->frontVector;
}

const glm::vec3& Camera::getRightVector()
{
    if (this->isValidVector == false)
    {
        this->computeVectors();
        this->computeViewMatrix();
    }

    return this->rightVector;
}

const glm::vec3& Camera::getUpVector()
{
    if (this->isValidVector == false)
    {
        this->computeVectors();
        this->computeViewMatrix();
    }

    return this->upVector;
}

const glm::vec3& Camera::getAngles()
{
    return this->angles;
}

void Camera::setAngles(const glm::vec3& a)
{
    this->angles = a;

    this->isValidVector     = false;
    this->isValidViewMatrix = false;
}

const glm::mat4& Camera::getViewMatrix()
{
    if (this->isValidViewMatrix == false)
        this->computeViewMatrix();

    return this->viewMatrix;
}


float Camera::getNear()
{
    return this->nearPlan;
}

void Camera::setNear(float near)
{
    this->nearPlan          = near;
    this->isValidProjMatrix = false;
}

float Camera::getFar()
{
    return this->farPlan;
}

void Camera::setFar(float far)
{
    this->farPlan           = far;
    this->isValidProjMatrix = false;
}

float Camera::getFov()
{
    return this->fov;
}

void Camera::setFov(float fov)
{
    this->fov               = fov;
    this->isValidProjMatrix = false;
}

const glm::mat4& Camera::getProjMatrix(const glm::ivec2& viewportSize)
{
    if ((this->viewportSize != viewportSize) || (this->isValidProjMatrix == false))
        this->computeProjMatrix(viewportSize);

    return this->projMatrix;
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

void Camera::computeAngles()
{
    this->angles.x = std::asin(this->frontVector.y);
    this->angles.y = std::acos(this->frontVector.z / std::cos(this->angles.x));
    this->angles.z = 0;
}

void Camera::computeVectors()
{
    // Front vector
    this->frontVector.x = std::cos(this->angles.x) * std::sin(this->angles.y);
    this->frontVector.y = std::sin(this->angles.x);
    this->frontVector.z = std::cos(this->angles.x) * std::cos(this->angles.y);
    
    // Right vector
    this->rightVector.x = std::sin(this->angles.y - 0.5 * M_PI);
    this->rightVector.y = 0;
    this->rightVector.z = std::cos(this->angles.y - 0.5 * M_PI);

    // Up vector
    this->upVector = glm::cross(this->rightVector, this->frontVector);

    this->isValidVector = true;
}

void Camera::computeViewMatrix()
{
    this->viewMatrix        = glm::lookAt(this->position, this->position + this->frontVector, this->upVector);
    this->isValidViewMatrix = true;
}

void Camera::computeProjMatrix(const glm::ivec2& viewportSize)
{
    this->viewportSize      = viewportSize;
    this->projMatrix        = glm::perspective(this->fov, float(viewportSize.x) / viewportSize.y, this->nearPlan, this->farPlan);
    this->isValidProjMatrix = true;
}