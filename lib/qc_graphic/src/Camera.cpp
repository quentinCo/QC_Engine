#define _USE_MATH_DEFINES
#define NOMINMAX
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>

#include <qc_graphic/Camera.hpp>

using namespace qc_graphic;

Camera::Camera(const glm::vec3& pos , float n, float f, float fov, float width, float height, const glm::vec3& front)
    : width(width), height(height), nearPlan(n), farPlan(f), fov(fov), frontVector(front), isModified(false)
{
    glm::normalize(frontVector);

    glm::vec3 rotation = { 0,0,0 };
    rotation.x = std::asin(frontVector.y);
    rotation.y = std::acos(frontVector.z / std::cos(rotation.y));
    transformation.setRotate(rotation);

    updateCameraProperties();
}

void Camera::updateCameraProperties()
{
    const glm::vec3& position = transformation.getPosition();

    computeVectors();
    aspectRatio = width / height;

    projMatrix = glm::perspective(fov, aspectRatio, nearPlan, farPlan);
    viewMatrix = glm::lookAt(position, position + frontVector, upVector);
}

void Camera::computeVectors()
{
    const glm::vec3& rotation = transformation.getRotation();

    frontVector = glm::vec3(std::cos(rotation.x)*std::sin(rotation.y), std::sin(rotation.x), std::cos(rotation.x)*std::cos(rotation.y));
    leftVector  = glm::vec3(std::sin(rotation.y + M_PI * 0.5), 0, std::cos(rotation.y + M_PI * 0.5));
    upVector    = glm::cross(frontVector, leftVector);
}