#pragma once

#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <glm\mat4x4.hpp>

namespace qc
{

class Transform
{
public:
    // Constructors
    Transform(const glm::vec3& position = glm::vec3(0), const glm::vec3& rotation = glm::vec3(0), const glm::vec3& scale = glm::vec3(1));
    
    ~Transform() {}

    // Getters - Setters
    const glm::vec4& getPosition() const
    {
        return this->position;
    }

    void setPosition(const glm::vec3& position);

    const glm::vec3& getOrientation() const
    {
        return this->orientation;
    }

    void setOrientation(const glm::vec3& rotation);

    void setOrientationDeg(const glm::vec3& rotation);

    const glm::vec3& getScale() const
    {
        return this->scaling;
    }

    void setScale(const glm::vec3& scale);

    const glm::mat4& getMatrix();
    const glm::mat4& getPositionMatrix();
    const glm::mat4& getRotateMatrix();
    const glm::mat4& getScalingMatrix();

    // Function
    void translate(const glm::vec3& translation);   // Add translation to this->position

    void rotate(const glm::vec3& rotation);         // Add rotation (Rad) to this->orientation

    void rotateDeg(const glm::vec3& rotation);         // Add rotation (Deg) to this->orientation

    void scale(const glm::vec3& scaling);           // Multiply this->scaling._ by scaling._
    void scale(float scaling);

    bool isValid() const
    {
        return this->isValidMatrix;
    }


private:
    glm::vec4   position;
    glm::vec3   orientation;                        // Radiante angle
    glm::vec3   scaling;

    glm::mat4   matrix;                             // Matrix of all transformations
    glm::mat4   positionMatrix;
    glm::mat4   rotateMatrix;
    glm::mat4   scalingMatrix;

    bool        isValidMatrix;

    // Functions
    void computeMatrix();
};

}   //! namespace qc