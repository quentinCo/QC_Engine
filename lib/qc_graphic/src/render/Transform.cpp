#include <qc_graphic\Transform.hpp>

#include <iostream>

#include <glm\trigonometric.hpp>
#include <glm\gtc\matrix_transform.hpp>

using namespace qc;

/*-------------------- TRANSFOM CONSTRUCTORS ---------------------------------*/
Transform::Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
    : position(position, 1), orientation(rotation), scaling(scale), matrix(), isValidMatrix(false)
{
    this->computeMatrix();
}

/*-------------------- TRANSFOM GETTERS / SETTERS ---------------------------------*/
void Transform::setPosition(const glm::vec3& position)
{
    this->position = glm::vec4(position, 1);

    this->isValidMatrix = false;
}

void Transform::setOrientation(const glm::vec3& rotation)
{
    this->orientation = glm::vec4(rotation, 0);

    this->isValidMatrix = false;
}

void Transform::setOrientationDeg(const glm::vec3& rotation)
{
    glm::vec3 radRotation   = glm::radians(rotation);
    this->orientation       = glm::vec4(radRotation, 0);

    this->isValidMatrix = false;
}

void Transform::setScale(const glm::vec3& scale)
{
    this->scaling = glm::vec4(scale, 0);

    this->isValidMatrix = false;
}

const glm::mat4& Transform::getMatrix()
{
    if (this->isValidMatrix == false)
        this->computeMatrix();

    return this->matrix;
}

const glm::mat4& Transform::getPositionMatrix()
{
    if (this->isValidMatrix == false)
        this->computeMatrix();

    return this->positionMatrix;
}

const glm::mat4& Transform::getRotateMatrix()
{
    if (this->isValidMatrix == false)
        this->computeMatrix();

    return this->rotateMatrix;
}

const glm::mat4& Transform::getScalingMatrix()
{
    if (this->isValidMatrix == false)
        this->computeMatrix();

    return this->scalingMatrix;
}

/*-------------------- TRANSFOM FUNCTIONS ---------------------------------*/
void Transform::translate(const glm::vec3& translation)
{
    this->position.x += translation.x;
    this->position.y += translation.y;
    this->position.z += translation.z;
    this->isValidMatrix = false;
}

void Transform::rotate(const glm::vec3& rotation)
{
    this->orientation += rotation;
    this->isValidMatrix = false;
}

void Transform::rotateDeg(const glm::vec3& rotation)
{
    this->orientation += glm::radians(rotation);
    this->isValidMatrix = false;
}

void Transform::scale(const glm::vec3& scaling)
{
    this->scaling.x *= scaling.x;
    this->scaling.y *= scaling.y;
    this->scaling.z *= scaling.z;

    this->isValidMatrix = false;
}

void Transform::scale(float scaling)
{
    this->scaling *= scaling;

    this->isValidMatrix = false;
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
void Transform::computeMatrix()
{
    this->positionMatrix    = glm::mat4();
    this->rotateMatrix     = glm::mat4();
    this->scalingMatrix     = glm::mat4();
    
    // Translate
    this->positionMatrix[3] = this->position;

    // Rotation
    glm::vec3 cos;
    cos.x = glm::cos(this->orientation.x);
    cos.y = glm::cos(this->orientation.y);
    cos.z = glm::cos(this->orientation.z);

    glm::vec3 sin;
    sin.x = glm::sin(this->orientation.x);
    sin.y = glm::sin(this->orientation.y);
    sin.z = glm::sin(this->orientation.z);

    this->rotateMatrix[0][0] = cos.z * cos.y - sin.z * sin.x * sin.y;
    this->rotateMatrix[0][1] = sin.z * cos.y + cos.z * sin.x * sin.y;
    this->rotateMatrix[0][2] = -1 * cos.x * sin.y;

    this->rotateMatrix[1][0] = -1 * sin.z * cos.x;
    this->rotateMatrix[1][1] = cos.z * cos.x;
    this->rotateMatrix[1][2] = sin.x;

    this->rotateMatrix[2][0] = cos.z * sin.y + sin.z * sin.x * cos.y;
    this->rotateMatrix[2][1] = sin.z * sin.y - cos.z * sin.x * cos.y;
    this->rotateMatrix[2][2] = cos.x * cos.y;

    // Scale
    this->scalingMatrix[0][0] = this->scaling.x;
    this->scalingMatrix[1][1] = this->scaling.y;
    this->scalingMatrix[2][2] = this->scaling.z;

    this->matrix = this->positionMatrix * this->rotateMatrix * this->scalingMatrix;
    /*
    this->matrix = glm::mat4();
    glm::vec3 pos = this->position;
    this->matrix = glm::rotate(this->matrix, this->orientation.x, glm::vec3(1, 0, 0));
    this->matrix = glm::rotate(this->matrix, this->orientation.y, glm::vec3(0, 1, 0));
    this->matrix = glm::rotate(this->matrix, this->orientation.z, glm::vec3(0, 0, 1));
    this->matrix = glm::translate(this->matrix, pos);
    */

    this->isValidMatrix = true;
}