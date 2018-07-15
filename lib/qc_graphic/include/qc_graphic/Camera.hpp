#pragma once

#include <glm/glm.hpp>

#include <qc_graphic/Geometry/Transformation.hpp>

namespace qc_graphic
{

class Camera
{
public:
    Camera(const glm::vec3& pos = { 0,0,0 }, float near = 0.01f, float far = 100.f, float fov = 60.f, float width = 720.f, float height = 1280.f, const glm::vec3& front = { 0,0,-1 });

    geometry::Transformation& getTransfomation()
    {
        if (isModified || transformation.hasBeenModified())
            updateCameraProperties();
        return transformation;
    }

    float getWidth()
    {
        if (isModified || transformation.hasBeenModified())
            updateCameraProperties();
        return width;
    }

    float getHeight()
    {
        if (isModified || transformation.hasBeenModified())
            updateCameraProperties();
        return height;
    }

    float getAspectRatio()
    {
        if (isModified || transformation.hasBeenModified())
            updateCameraProperties();
        return aspectRatio;
    }

    float getNear()
    {
        if (isModified || transformation.hasBeenModified())
            updateCameraProperties();
        return nearPlan;
    }

    float getFar()
    {
        if (isModified || transformation.hasBeenModified())
            updateCameraProperties();
        return farPlan;
    }

    float getFov()
    {
        if (isModified || transformation.hasBeenModified())
            updateCameraProperties();
        return fov;
    }

    const glm::vec3& getFrontVector()
    {
        if (isModified || transformation.hasBeenModified())
            updateCameraProperties();
        return frontVector;
    }

    const glm::vec3& getUpVector()
    {
        if (isModified || transformation.hasBeenModified())
            updateCameraProperties();
        return upVector;
    }

    const glm::vec3& getLeftVector()
    {
        if (isModified || transformation.hasBeenModified())
            updateCameraProperties();
        return leftVector;
    }

    const glm::mat4& getViewMatrix()
    {
        if (isModified || transformation.hasBeenModified())
            updateCameraProperties();
        return viewMatrix;
    }

    const glm::mat4& getProjMatrix()
    {
        if (isModified || transformation.hasBeenModified())
            updateCameraProperties();
        return projMatrix;
    }

    void setWidth(float width)
    {
        this->width = width;
        isModified = true;
    }

    void setHeight(float height)
    {
        this->height = height;
        isModified = true;
    }

    void setNear(float n)
    {
        this->nearPlan = n;
        isModified = true;
    }

    void setFar(float f)
    {
        this->farPlan = f;
        isModified = true;
    }

    void setFov(float fov)
    {
        this->fov = fov;
        isModified = true;
    }

    bool hasBeenModified()
    {
        return isModified || transformation.hasBeenModified();
    }

private:
    geometry::Transformation    transformation;

    float                       width;
    float                       height;
    float                       aspectRatio;
    float                       nearPlan;
    float                       farPlan;
    float                       fov;

    glm::vec3                   frontVector;
    glm::vec3                   upVector;
    glm::vec3                   leftVector;

    glm::mat4                   viewMatrix;
    glm::mat4                   projMatrix;

    bool                        isModified;

    void updateCameraProperties();
    void computeVectors();
};

}