#pragma once

#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

namespace qc_graphic
{

namespace geometry
{

class Transformation
{

public:
    Transformation()
        : position(0), rotation(0), scale(1), transformMatrix(1), modify(false)
    {}

    Transformation(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
        : position(position), rotation(rotation), scale(scale), transformMatrix(1), modify(false)
    {
        computeTransformMatrix();
    }


    const glm::vec3& getPosition()
    {
        return position;
    }

    const glm::vec3& getRotation()
    {
        return rotation;
    }

    const glm::vec3& getScale()
    {
        return scale;
    }
    
    const glm::mat4& getTransformMatrix()
    {
        if(modify)
            computeTransformMatrix();
        
        return transformMatrix;
    }

    
    void setPosition(const glm::vec3& pos)
    {
        this->position = pos;
        modify = true;
    }

    void setPosition(float x, float y, float z)
    {
        this->position = glm::vec3(x, y, z);
        modify = true;
    }

    void setRotate(const glm::vec3& r)
    {
        this->rotation = r;
        modify = true;
    }

    void setRotate(float x, float y, float z)
    {
        this->rotation = glm::vec3(x, y, z);
        modify = true;
    }

    void setScale(const glm::vec3& s)
    {
        this->scale = s;
        modify = true;
    }

    void setScale(float x, float y, float z)
    {
        x = std::max(x, 1.f);
        y = std::max(y, 1.f);
        z = std::max(z, 1.f);
        this->scale = glm::vec3(x, y, z);
        modify = true;
    }



    void translate(const glm::vec3& pos)
    {
        this->position += pos;
        modify = true;
    }

    void translate(float x, float y, float z)
    {
        this->position.x += x;
        this->position.y += y;
        this->position.z += z;
        modify = true;
    }

    void rotate(const glm::vec3& r)
    {
        this->rotation += r;
        modify = true;
    }

    void rotate(float x, float y, float z)
    {
        rotation.x += x;
        rotation.y += y;
        rotation.z += z;
        modify = true;
    }

private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::mat4 transformMatrix;

    bool modify;

    void computeTransformMatrix()
    {
        transformMatrix = glm::translate(glm::mat4(1), position);

        transformMatrix = glm::rotate(transformMatrix, rotation.x, glm::vec3(1, 0, 0));
        transformMatrix = glm::rotate(transformMatrix, rotation.y, glm::vec3(0, 1, 0));
        transformMatrix = glm::rotate(transformMatrix, rotation.z, glm::vec3(0, 0, 1));

        transformMatrix = glm::scale(transformMatrix, scale);

        modify = false;
    }

};

}

}