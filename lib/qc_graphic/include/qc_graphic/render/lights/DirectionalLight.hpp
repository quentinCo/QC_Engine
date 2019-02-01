#pragma once

#include <qc_graphic\render\lights\Light.hpp>

#include <glm\vec4.hpp>

namespace qc
{

namespace render
{

namespace light
{

class DirectionalLight: public Light
{
public:
    DirectionalLight(const glm::vec3& direction = glm::vec3(0), float intensity = 1, const glm::vec3& color = glm::vec3(1))
        : direction(direction, 1), intensity(intensity), color(color, 1)
    {}
    
    virtual float getIntensity() const override
    {
        return this->intensity;
    }

    virtual void setIntensity(float intensity) override
    {
        this->intensity = intensity;
    }

    virtual const glm::vec4& getColor() const override
    {
        return this->color;
    }

    virtual void setColor(const glm::vec3& color) override
    {
        this->color = glm::vec4(color, 1);
    }

    const glm::vec4& getDirection() const
    {
        return this->direction;
    }

    void setDirection(const glm::vec3& direction)
    {
        this->direction = glm::vec4(direction, 0);
    }

    void setDirection(const glm::vec4& direction)
    {
        this->direction = direction;
        this->direction.w = 0;
    }

private:
    glm::vec4   direction;
    glm::vec4   color;
    float       intensity;
};

}   //! namespace light

}   //! namespace render

}   //! namespace qc